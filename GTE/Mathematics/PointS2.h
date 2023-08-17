
#pragma once

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

        PointS2(Real latIn, Real lonIn)
        {
            LogAssert(latIn >= -GTE_C_HALF_PI && latIn <= GTE_C_HALF_PI, "Invalid lat.");
            lat = latIn;
            LogAssert(lonIn >= -GTE_C_PI && lonIn <= GTE_C_PI, "Invalid lon.");
            lon = lonIn;
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
}