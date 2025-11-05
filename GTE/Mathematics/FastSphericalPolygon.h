#pragma once

#include <Mathematics/SphericalPolygon.h>
#include <algorithm>

namespace gte
{
    template <typename Real>
    class FastSphericalPolygon
    {
    public:
        using Vec3 = Vector3<Real>;

        FastSphericalPolygon(std::vector<Vec3> verts, Vec3 interiorHint)
            : mBase(std::move(verts), interiorHint)
        {
            BuildPreprocessor();
        }

        FastSphericalPolygon(std::vector<PointS2<Real>> const& verts, PointS2<Real> interiorHint)
            : mBase(verts, interiorHint)
        {
            BuildPreprocessor();
        }

        // Crossing counts

        // Returns the number of edge crossings for the query ray.
        // Returns -1 if the query lies on the boundary.
        int NumCrossings(PointS2<Real> const& q) const
        {
            return NumCrossings(GeographicToCart<Real>(q, (Real)1));
        }

        int NumCrossings(Vec3 const& qCartInWorld) const
        {
            auto const& QI = mBase.QI();
            Vec3 qQ = QI * qCartInWorld;

            PointS2<Real> qp = CartToGeographic<Real>(qQ);
            Real lon = detail::WrapLon0ToTwoPi<Real>(qp.Lon());
            Real lat = qp.Lat();

            auto const& sliceBounds = mSliceBounds;
            if (sliceBounds.size() < 2)
            {
                // Degenerate preprocessor; defer to base computation.
                return mBase.NumCrossings(qCartInWorld);
            }

            int start = 0;
            int end   = static_cast<int>(sliceBounds.size()); // [start,end)
            while ((end - start) != 1)
            {
                int mid = start + (end - start)/2;
                if (lon <= sliceBounds[size_t(mid)])
                    end = mid;
                else
                    start = mid;
            }
            auto const& candidates = mClassified[size_t(start)];

            int crossings = 0;
            auto const& edges = mBase.GetEdgesQ();
            for (int idx : candidates)
            {
                int c = edges[size_t(idx)].Contains(qQ, lon, lat);
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

        // Containment

        int Contains(PointS2<Real> const& q) const
        {
            return Contains(GeographicToCart<Real>(q, (Real)1));
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

        SphericalPolygon<Real> const& Base() const { return mBase; }

    private:

        void BuildPreprocessor()
        {
            // Copy and sort the [0,2pi) vertex longitudes. Force the last to 2pi
            mSliceBounds = mBase.GetLonArrayQ();
            if (mSliceBounds.size() < 2)
            {
                mClassified.clear();
                return;
            }

            std::sort(mSliceBounds.begin(), mSliceBounds.end());

            // Build index list for all edges.
            size_t E = mBase.GetEdgesQ().size();
            std::vector<int> parentIndices(E);
            for (size_t i = 0; i < E; ++i) parentIndices[i] = static_cast<int>(i);

            // Classified bins between consecutive slice bounds.
            mClassified.assign(mSliceBounds.size() - 1, {});

            // Recursively partition and classify
            PreprocessRecursive(parentIndices, 0, static_cast<int>(mSliceBounds.size() - 1));
        }

        void PreprocessRecursive(std::vector<int> const& parent, int start, int end)
        {
            auto child = ClassifyEdges(parent, start, end);

            if ((end - start) == 1)
            {
                mClassified[size_t(start)] = std::move(child);
                return;
            }

            int mid = start + (end - start)/2;
            PreprocessRecursive(child, start, mid);
            PreprocessRecursive(child, mid, end);
        }

        std::vector<int> ClassifyEdges(std::vector<int> const& parent, int start, int end) const
        {
            Real b1 = mSliceBounds[size_t(start)];
            Real b2 = mSliceBounds[size_t(end)];
            auto const& edges = mBase.GetEdgesQ();

            std::vector<int> child;
            child.reserve(parent.size());

            for (int idx : parent)
            {
                auto const& e = edges[size_t(idx)];

                Real v1 = e.Bound1();
                Real v2 = e.Bound2();

                // Two inclusion tests
                bool cond1 = (v1 >= b1 && v1 <= b2) || (v2 >= b1 && v2 <= b2);
                bool cond2 = detail::LonBounded(b1, b2, v1) || detail::LonBounded(b1, b2, v2);

                if (cond1 || cond2) child.push_back(idx);
            }
            return child;
        }

    private:
        SphericalPolygon<Real> mBase;
        std::vector<Real> mSliceBounds;
        std::vector<std::vector<int>> mClassified;
    };
}