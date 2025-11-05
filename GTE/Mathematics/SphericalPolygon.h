#pragma once

#include <Mathematics/Matrix.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/PointS2.h>

#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include <cmath>

namespace gte
{
    namespace detail
    {
        template <typename Real>
        inline Real WrapLon0ToTwoPi(Real lon)
        {
            // Convert [-pi, pi] to [0, 2pi)
            Real twoPi = static_cast<Real>(GTE_C_TWO_PI);
            Real x = std::fmod(lon, twoPi);
            if (x < (Real)0) x += twoPi;
            return x;
        }

        template <typename Real>
        inline bool NearlyEqual(Real a, Real b, Real eps = static_cast<Real>(1e-12))
        {
            return std::fabs(a - b) <= eps;
        }

        // Same as the old util::lonBounded(). Inputs are assumed in [0,2pi).
        // Returns true if 'lon' lies on the minor arc between [b1,b2] (inclusive of endpoints).
        template <typename Real>
        inline bool LonBounded(Real b1, Real b2, Real lon)
        {
            // b1 <= b2 by construction
            Real span = b2 - b1;
            bool inSegment = (lon >= b1 && lon <= b2);
            if (span < static_cast<Real>(GTE_C_PI))
                return inSegment;
            else
                return !inSegment; // complement is the minor arc
        }

        // Same as old util::latBounded:
        //  -1 : "on edge" (lat inside inclusive range),
        //  +2 : passes edge "above"
        //  -2 : doesn't pass edge
        template <typename Real>
        inline int LatBounded(Real b1, Real b2, Real lat)
        {
            // Inclusive check for being on the segment in latitude
            if (b2 > b1)
            {
                if (lat >= b1 && lat <= b2) return -1;
                else if (lat > b2) return +2;
                else return -2;
            }
            else // b1 >= b2
            {
                if (lat >= b2 && lat <= b1) return -1;
                else if (lat > b1) return +2;
                else return -2;
            }
        }

        template <typename Real>
        inline Vector3<Real> ToUnit(Vector3<Real> v)
        {
            Normalize(v, false);
            return v;
        }

        // Build the query-frame DCM QI: rows are x, y, z as in the old implementation.
        // z = contained, y = unit(z x v0), x = unit(y x z).
        // If z and v0 are nearly collinear, use a robust orthogonal complement.
        template <typename Real>
        inline Matrix<3,3,Real> BuildQI(Vector3<Real> contained, Vector3<Real> firstVertex)
        {
            Vector3<Real> z = ToUnit(contained);
            Vector3<Real> v0 = ToUnit(firstVertex);

            // Try the intended construction; fall back robustly if needed.
            Vector3<Real> y = Cross(z, v0);
            if (Length(y) < static_cast<Real>(1e-14))
            {
                // z ~ v0. pick any orthonormal pair orthogonal to z
                auto basis = ComputeOrthogonalComplement(z); // returns two vectors perpendicular to z
                Vector3<Real> x = basis[0];
                y = basis[1];
                Matrix<3,3,Real> QI;
                QI(0,0)=x[0]; QI(0,1)=x[1]; QI(0,2)=x[2];
                QI(1,0)=y[0]; QI(1,1)=y[1]; QI(1,2)=y[2];
                QI(2,0)=z[0]; QI(2,1)=z[1]; QI(2,2)=z[2];
                return QI;
            }

            Normalize(y, false);
            Vector3<Real> x = Cross(y, z);
            Normalize(x, false);

            Matrix<3,3,Real> QI;
            QI(0,0)=x[0]; QI(0,1)=x[1]; QI(0,2)=x[2];
            QI(1,0)=y[0]; QI(1,1)=y[1]; QI(1,2)=y[2];
            QI(2,0)=z[0]; QI(2,1)=z[1]; QI(2,2)=z[2];
            return QI;
        }

        template <typename Real>
        class Edge
        {
        public:
            Edge() = default;

            // Construct from already-transformed (query-frame) cartesian vertices.
            Edge(Vector3<Real> const& vq0, Vector3<Real> const& vq1)
            {
                // Spherical values (lat, lon) using geographic convention.
                PointS2<Real> p0 = CartToGeographic<Real>(vq0);
                PointS2<Real> p1 = CartToGeographic<Real>(vq1);

                lat1 = p0.Lat();
                lat2 = p1.Lat();

                // Store [0,2pi) longitudes for the minor-arc tests
                lon1 = WrapLon0ToTwoPi<Real>(p0.Lon());
                lon2 = WrapLon0ToTwoPi<Real>(p1.Lon());

                // Bounds are the ordered pair (minLon, maxLon)
                if (lon1 <= lon2)
                {
                    bound1 = lon1; bound2 = lon2;
                }
                else
                {
                    bound1 = lon2; bound2 = lon1;
                }

                // Great-circle pole and shooter reference (Z axis in query frame).
                pole = Cross(vq0, vq1);
                shooterDotPole = Dot(Vector3<Real>{ (Real)0, (Real)0, (Real)1 }, pole);
            }

            // Returns:
            //   +1 if the edge contributes a crossing,
            //    0 if not,
            //   -1 if the query is exactly on this edge.
            int Contains(Vector3<Real> const& queryQ, Real lonQ0_2pi, Real latQ) const
            {
                int bounds = BoundsPoint(lonQ0_2pi, latQ);
                if (bounds == 1)
                    return CrossesBoundary(queryQ);
                else if (bounds == 2)   // pq-aligned edge special case: count it
                    return 1;
                else if (bounds == -2)  // pq-aligned no-cross case
                    return 0;
                else if (bounds == -1)  // on edge vertically
                    return -1;
                return 0;
            }

            // Accessors used by the fast preprocessor.
            Real Bound1() const { return bound1; }
            Real Bound2() const { return bound2; }

        private:

            // Necessary strike condition
            // Returns:
            //   1  : eligible (lon within the edge’s minor-arc slab)
            //  -1  : “on edge” when vertex-lon hit and lat is within vertical span
            //   2  : special “passes edge” case for vertex alignment
            //  -2  : special “doesn’t pass” case for vertex alignment
            //   0  : not eligible
            int BoundsPoint(Real lonQ0_2pi, Real latQ) const
            {
                // Vertex longitudes coincident with the query ray direction
                bool onB1 = NearlyEqual(lonQ0_2pi, bound1);
                bool onB2 = NearlyEqual(lonQ0_2pi, bound2);

                if (onB1 || onB2)
                {
                    // Both equal, then treat as vertical span test (on-edge test)
                    if (onB1 && onB2)
                    {
                        return LatBounded(lat1, lat2, latQ);
                    }

                    // Choose the "far" endpoint to count to avoid double-counting,
                    if ((bound2 - bound1) < static_cast<Real>(GTE_C_PI))
                        return onB2 ? 1 : 0;
                    else
                        return onB1 ? 1 : 0;
                }

                return LonBounded(bound1, bound2, lonQ0_2pi) ? 1 : 0;
            }

            // Hemisphere check vs. shooter (Z-axis) in query frame
            int CrossesBoundary(Vector3<Real> const& queryQ) const
            {
                Real qDotPole = Dot(queryQ, pole);

                if (NearlyEqual(qDotPole, (Real)0))
                    return -1; // exactly on the edge plane

                return (qDotPole * shooterDotPole < (Real)0) ? 1 : 0;
            }

        private:
            Vector3<Real> pole{};       // great-circle pole
            Real shooterDotPole{};      // dot( (0,0,1), pole )

            // Stored for vertex-aligned special handling
            Real lat1{}, lat2{};
            Real lon1{}, lon2{};
            Real bound1{}, bound2{};
        };
    }

    // SphericalPolygon: one-shot (non-preprocessed) containment queries
    template <typename Real>
    class SphericalPolygon
    {
    public:
        using Vec3 = Vector3<Real>;
        using Mat3 = Matrix<3,3,Real>;

        // Construct from cartesian unit vectors. If polygon is not closed,
        // it will be closed automatically. 
        // 'interiorHint' should be a point known to be inside the polygon.
        SphericalPolygon(std::vector<Vec3> verticesIn, Vec3 interiorHint)
        {
            InitFromCartesian(verticesIn, interiorHint);
        }

        // Construct from geographic vertices (lat,lon) and convert to cartesian.
        SphericalPolygon(std::vector<PointS2<Real>> const& verticesIn, PointS2<Real> interiorHint)
        {
            std::vector<Vec3> cart;
            cart.reserve(verticesIn.size());
            for (auto const& p : verticesIn)
                cart.push_back(GeographicToCart<Real>(p, (Real)1));
            InitFromCartesian(cart, GeographicToCart<Real>(interiorHint, (Real)1));
        }

        // Returns the number of edge crossings for the query ray.
        // Returns -1 if the query lies on the boundary.
        int NumCrossings(PointS2<Real> const& q) const
        {
            Vec3 qCart = GeographicToCart<Real>(q, (Real)1);
            return NumCrossings(qCart);
        }

        int NumCrossings(Vec3 const& qCartInWorld) const
        {
            Vec3 qCart = mQI * qCartInWorld;

            PointS2<Real> qp = CartToGeographic<Real>(qCart);
            Real lonQ = detail::WrapLon0ToTwoPi<Real>(qp.Lon());
            Real latQ = qp.Lat();

            int crossings = 0;
            for (auto const& e : mEdgesQ)
            {
                int c = e.Contains(qCart, lonQ, latQ);
                if (c == -1) return -1;
                crossings += c;
            }
            return crossings;
        }

        std::vector<int> NumCrossings(std::vector<Vec3> const& queries) const
        {
            std::vector<int> out;
            out.reserve(queries.size());
            for (auto const& q : queries) out.push_back(NumCrossings(q));
            return out;
        }

        std::vector<int> NumCrossings(std::vector<PointS2<Real>> const& queries) const
        {
            std::vector<int> out;
            out.reserve(queries.size());
            for (auto const& q : queries) out.push_back(NumCrossings(q));
            return out;
        }

        // Return -1 (on boundary), 1 (inside), 0 (outside).
        int Contains(PointS2<Real> const& q) const
        {
            Vec3 qCart = GeographicToCart<Real>(q, (Real)1);
            return Contains(qCart);
        }

        int Contains(Vec3 const& qCartInWorld) const
        {
            int crossings = NumCrossings(qCartInWorld);
            if (crossings == -1) return -1;
            return ((crossings % 2) == 0) ? 1 : 0;
        }

        std::vector<int> Contains(std::vector<Vec3> const& queries) const
        {
            std::vector<int> out;
            out.reserve(queries.size());
            for (auto const& q : queries) out.push_back(Contains(q));
            return out;
        }

        std::vector<int> Contains(std::vector<PointS2<Real>> const& queries) const
        {
            std::vector<int> out;
            out.reserve(queries.size());
            for (auto const& q : queries) out.push_back(Contains(q));
            return out;
        }

        // Sets the orientation of the spherical polygon. The spherical
        // polygon was initialy specified in the Initial frame I. This functions
        // sets the orientation of the polygon to the frame N by supplying the matrix IN
        // which transforms N-frame coordinates to I-frame coordinates.
        void SetTransform(Mat3 const& IN)
        {
            mQI = mQI * IN;
        }

        // Data access for preprocessors

        std::vector<Real> GetLonArrayQ() const
        {
            return mLonQ; // already in [0,2pi)
        }

        std::vector<detail::Edge<Real>> const& GetEdgesQ() const
        {
            return mEdgesQ;
        }

        Mat3 const& QI() const { return mQI; }

        // Origin of the spherical polygon. All vectors in this
        // class are assumed to be specified relative to this origin.
        // It is left public for easy access/modification.
        Vec3 origin = Vec3{0,0,0};

    private:
        void InitFromCartesian(std::vector<Vec3> vertices, Vec3 interiorHint)
        {
            // Normalize all vertices
            for (auto& v : vertices) Normalize(v, false);

            // Close polygon if needed (old code expected the last = first).
            if (vertices.size() >= 2)
            {
                Vec3 first = vertices.front();
                Vec3 last  = vertices.back();
                if (Length(first - last) > static_cast<Real>(1e-12))
                    vertices.push_back(first);
            }

            // Build the query-frame DCM using the first vertex and interior hint.
            mQI = detail::BuildQI<Real>(interiorHint, vertices.front());

            // Transform vertices to query frame Q and build edges.
            std::vector<Vec3> vQ(vertices.size());
            for (size_t i = 0; i < vertices.size(); ++i)
                vQ[i] = mQI * vertices[i];

            BuildEdgesAndLons(vQ);
        }

        void BuildEdgesAndLons(std::vector<Vec3> const& vQ)
        {
            size_t N = vQ.size();
            if (N < 2) { mEdgesQ.clear(); mLonQ.clear(); return; }

            // Longitudes for preprocessor; use vertex longitudes in Q.
            mLonQ.resize(N);
            for (size_t i = 0; i < N; ++i)
            {
                PointS2<Real> p = CartToGeographic<Real>(vQ[i]);
                mLonQ[i] = detail::WrapLon0ToTwoPi<Real>(p.Lon());
            }

            if (!mLonQ.empty())
            {
                mLonQ.back() = static_cast<Real>(GTE_C_TWO_PI);
            }

            // Edges between consecutive vertices (N-1)
            mEdgesQ.resize(N - 1);
            for (size_t i = 0; i + 1 < N; ++i)
            {
                mEdgesQ[i] = detail::Edge<Real>(vQ[i], vQ[i+1]);
            }
        }

    private:
        Matrix<3,3,Real> mQI{};
        std::vector<detail::Edge<Real>> mEdgesQ;
        std::vector<Real> mLonQ; // [0,2pi), last element forced to 2pi
    };
}