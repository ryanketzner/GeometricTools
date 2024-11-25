
#pragma once

#include <cmath>

#include <Mathematics/Math.h>
#include <Mathematics/Logger.h>
#include <Mathematics/Vector3.h>

namespace gte
{
    template <typename Real>
    class PointS2
    {
    public:

        PointS2() : 
        lat((Real)0),
        lon((Real)0)
        {
        }

        PointS2(Real latIn, Real lonIn, bool enforce_bounds = true, bool degrees = false)
        {
            if (degrees)
            {
                latIn = latIn*GTE_C_PI/180.0;
                lonIn = lonIn*GTE_C_PI/180.0;
            }

            if (enforce_bounds)
            {
                LogAssert(latIn >= -GTE_C_HALF_PI && latIn <= GTE_C_HALF_PI, "Invalid lat.");
                lat = latIn;
                LogAssert(lonIn >= -GTE_C_PI && lonIn <= GTE_C_PI, "Invalid lon.");
                lon = lonIn;
            }
            else
            {
                lon = fmod(lonIn, GTE_C_TWO_PI);
                if (lon > GTE_C_PI)
                    lon -= GTE_C_TWO_PI;
                else if (lon < -GTE_C_PI)
                    lon += GTE_C_TWO_PI;

                LogAssert(latIn >= -GTE_C_HALF_PI && latIn <= GTE_C_HALF_PI, "Invalid lat.");
                lat = latIn;
            }
        }

        Real Lat() const
        {
            return lat;
        }

        Real Lon() const
        {
            return lon;
        }
    
    private:

        Real lat,lon;
    };

    // Takes arbitrary cartesian vector v as input
    // returns the longitude and latitude of v, which is guaranteed to be in
    // the range [-Pi/2, Pi/2] and [-Pi to Pi]
    template <typename Real>
    PointS2<Real> CartToGeographic(Vector3<Real> const& v)
    {
        // atan2 returns value from -Pi to Pi
        Real lon = std::atan2(v[1], v[0]);
        // asin returns value from -Pi/2 to Pi/2
        Real lat = std::asin(v[2]/Length(v));

        return PointS2<Real>(lat, lon);
    }

    template <typename Real>
    Vector3<Real> GeographicToCart(PointS2<Real> const& p, Real radius = 1.0)
    {
        Real lat = p.Lat();
        Real lon = p.Lon();

        Real x = std::cos(lat) * std::cos(lon);
        Real y = std::cos(lat) * std::sin(lon);
        Real z = std::sin(lat);

        return Vector3<Real>({radius*x, radius*y, radius*z});
    }

    // Converts Cartesian coordinates to geodetic coordinates (latitude and longitude)
    // Default values assume the WGS84 ellipsoid.
    template <typename Real>
    PointS2<Real> CartToGeodetic(Vector3<Real> const& v, Real a = 6378137.0, Real f = 1.0/298.257223563, Real tolerance = 1e-12)
    {
        // WGS84 ellipsoid constants
        const Real e2 = f * (2.0 - f); // First eccentricity squared

        // Cartesian coordinates
        Real x = v[0];
        Real y = v[1];
        Real z = v[2];

        // Compute longitude (λ)
        Real lon = std::atan2(y, x);

        // Compute hypotenuse (p)
        Real p = std::sqrt(x * x + y * y);

        // Handle special case when p is near zero (point at the pole)
        if (p <= std::numeric_limits<Real>::epsilon())
        {
            Real lat = (z >= (Real)0) ? GTE_C_HALF_PI : -GTE_C_HALF_PI;
            return PointS2<Real>(lat, lon);
        }

        // Initial approximation of latitude (φ)
        Real lat = std::atan2(z, p * (1 - e2));

        // Iteratively improve the latitude value
        const int maxIterations = 10;
        int iteration = 0;
        Real sinLat;
        Real N;
        Real latPrev;

        do
        {
            latPrev = lat;
            sinLat = std::sin(lat);
            N = a / std::sqrt(1 - e2 * sinLat * sinLat);
            lat = std::atan2(z + e2 * N * sinLat, p);
            iteration++;
        } while (std::abs(lat - latPrev) > tolerance && iteration < maxIterations);

        if (iteration == maxIterations)
            throw std::runtime_error("Routine to compute geodetic latitude failed to converge");

        return PointS2<Real>(lat, lon);
    }

    // Converts Geodetic coordinates to Cartesian. h is the elevation.
    // Default values assume the WGS84 ellipsoid.
    template <typename Real>
    Vector3<Real> GeodeticToCart(PointS2<Real> const& p, Real h = 0.0, Real a = 6378137.0, Real f = 1.0/298.257223563)
    {
        // WGS84 ellipsoid constants
        Real e2 = f * (2.0 - f); // First eccentricity squared

        Real lat = p.Lat();
        Real lon = p.Lon();

        Real sinLat = std::sin(lat);
        Real cosLat = std::cos(lat);
        Real sinLon = std::sin(lon);
        Real cosLon = std::cos(lon);

        // Radius of curvature in the prime vertical
        Real N = a / std::sqrt(1 - e2 * sinLat * sinLat);

        Real x = (N + h) * cosLat * cosLon;
        Real y = (N + h) * cosLat * sinLon;
        Real z = (N * (1 - e2) + h) * sinLat;

        return Vector3<Real>({x, y, z});
    }
}