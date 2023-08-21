
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

        PointS2(Real latIn, Real lonIn, bool enforce_bounds = true)
        {
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

    #include <cmath>

    template <typename Real>
    Vector3<Real> GeographicToCart(PointS2<Real> const& p)
    {
        Real lat = p.Lat();
        Real lon = p.Lon();

        Real x = std::cos(lat) * std::cos(lon);
        Real y = std::cos(lat) * std::sin(lon);
        Real z = std::sin(lat);

        return Vector3<Real>({x, y, z});
    }

}