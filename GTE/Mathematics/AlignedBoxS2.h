// Addon to GeometricTools

#pragma once

#include <array>
#include "Mathematics/Math.h"
#include "Mathematics/PointS2.h"

namespace gte
{
    template <typename Real>
    Real CounterclockwiseDist(Real lon1, Real lon2)
    {
        if (lon2 >= lon1)
            return lon2 - lon1;
        else
            return lon2 - lon1 + (Real)GTE_C_TWO_PI;
    }

    template <typename Real>
    Real ClockwiseDist(Real lon1, Real lon2)
    {
        if (lon1 >= lon2)
            return lon1 - lon2;
        else
            return lon1 - lon2 + (Real)GTE_C_TWO_PI;
    }

    template <typename Real>
    class AlignedBoxS2
    {

        public:

        // Default constructor gives the empty box
        AlignedBoxS2()
            :
            latMin((Real)GTE_C_HALF_PI),
            latMax((Real)-GTE_C_HALF_PI),
            lonMin((Real)GTE_C_PI),
            lonMax((Real)-GTE_C_PI)
        {
        }

        AlignedBoxS2(Real inLatMin, Real inLatMax, Real inLonMin, Real inLonMax)
        {
            latMin = inLatMin;
            latMax = inLatMax;
            lonMin = inLonMin;
            lonMax = inLonMax;
        }

        AlignedBoxS2(PointS2<Real> min, PointS2<Real> max)
        {
            latMin = min.Lat();
            latMax = max.Lat();
            lonMin = min.Lon();
            lonMax = max.Lon();
        }

        // Empty lon interval goes from Pi to -Pi (180 deg to -180 deg)
        static AlignedBoxS2 Empty()
        {
            return AlignedBoxS2((Real)GTE_C_HALF_PI,
                (Real)-GTE_C_HALF_PI, (Real)GTE_C_PI, (Real)-GTE_C_PI);
        }

        // Full lon interval goes from -Pi to Pi (-180 deg to 180 deg)
        static AlignedBoxS2 Full()
        {
            return AlignedBoxS2((Real)-GTE_C_HALF_PI, (Real)GTE_C_HALF_PI, 
                (Real)-GTE_C_PI, (Real)GTE_C_PI);
        }

        Real Area() const
        {
            if (IsEmpty())
                return (Real)0;
            else
                return LonLength()*(sin(latMax) - sin(latMin));
        }

        Real AreaRatio() const
        {
            return Area()/(4*GTE_C_PI);
        }

        // Returns the longitudinal length of the box in radians
        Real LonLength() const
        {
            return CounterclockwiseDist(lonMin,lonMax);
        }

        // Returns the latitudinal length of the box in radians
        Real LatLength() const
        {
            if (IsEmptyLat())
                return (Real)0;
            else
                return latMax - latMin;
        }

        // Empty lon interval goes from Pi/2 to -Pi/2 (180 to -180 deg)
        bool IsEmpty() const
        {
            return IsEmptyLat() && IsEmptyLon();
        }

        bool IsEmptyLat() const
        {
            return latMin > latMax;
        }

        bool IsEmptyLon() const
        {
            return lonMin == (Real)GTE_C_PI && lonMax == (Real)-GTE_C_PI;
        }

        // Returns true if either the latitude or longitude
        // interval defining the box is empty
        bool IsDegenerate() const
        {
            return (latMin > latMax) || (lonMin == (Real)GTE_C_PI && 
                lonMax == (Real)-GTE_C_PI);
        }

        // Returns true if the box spans a longitude greater than 180 degrees
        bool IsWide() const
        {
            return LonLength() >= GTE_C_PI;
        }

        // Expand the box so that it contains the specified point
        bool Expand(Real lat, Real lon)
        {
            if (lon == -GTE_C_PI)
                lon = GTE_C_PI;

            bool expandedLat = ExpandLat(lat);
            bool expandedLon = ExpandLon(lon);
            return  expandedLat || expandedLon;    
        }

        // Expand the box so that it contains the specified point
        bool ExpandLong(Real lat, Real lon)
        {
            if (lon == -GTE_C_PI)
                lon = GTE_C_PI;

            bool expandedLat = ExpandLat(lat);
            bool expandedLon = ExpandLonLong(lon);
            return  expandedLat || expandedLon;    
        }

        bool ExpandLon(Real lon)
        {
            if (ContainsLon(lon))
                return false;
            else
            {
                // Expand longitude interval in whichever direction requires
                // the least increase in length

                if (IsEmptyLon())
                    lonMin = lonMax = lon;
                else if (CounterclockwiseDist(lonMax,lon) <= 
                    ClockwiseDist(lonMin,lon))
                    lonMax = lon;
                else  
                    lonMin = lon;

                return true;
            }
        }

        bool ExpandLonLong(Real lon)
        {
            if (ContainsLon(lon))
                return false;
            else
            {
                // Expand longitude interval in whichever direction requires
                // the least increase in length

                if (IsEmptyLon())
                    lonMin = lonMax = lon;
                else if (CounterclockwiseDist(lonMax,lon) <= 
                    ClockwiseDist(lonMin,lon))
                    lonMin = lon;
                else  
                    lonMax = lon;

                return true;
            }
        }

        bool ExpandLat(Real lat)
        {
            bool lowerExpanded = false, upperExpanded = false;

            if (lat < latMin)
            {
                latMin = lat;
                lowerExpanded = true;
            }
            
            if (lat > latMax)
            {
                latMax = lat;
                upperExpanded = true;
            }

            return lowerExpanded || upperExpanded;
        }

        bool ContainsLat(Real lat) const
        {
            return lat >= latMin && lat <= latMax;            
        }

        bool ContainsLon(Real lon) const
        {
            if (IsInverted())
                return lon >= lonMin || lon <= lonMax && !IsEmptyLon();
            else
                return lon >= lonMin && lon <= lonMax;
        }

        bool IsInverted() const
        {
            return lonMin > lonMax;
        }

        void Join()
        {
            lonMin = -GTE_C_PI;
            lonMax = GTE_C_PI;
        }

        void ToNorthPolarCap()
        {
            latMax = GTE_C_HALF_PI;
            lonMin = -GTE_C_PI;
            lonMax = GTE_C_PI;
        }

        void ToSouthPolarCap()
        {
            latMin = -GTE_C_HALF_PI;
            lonMin = -GTE_C_PI;
            lonMax = GTE_C_PI;
        }

        std::array<AlignedBoxS2<Real>,2> Split() const
        {
            AlignedBoxS2<Real> first(latMin,latMax,lonMin, (Real)GTE_C_PI);
            AlignedBoxS2<Real> second(latMin,latMax, (Real)-GTE_C_PI, lonMax);
            return {first,second};
        }

        Real latMin, latMax, lonMin, lonMax;

    public:
        // Comparisons to support sorted containers

        bool operator==(AlignedBoxS2 const& box) const
        {
            return latMin == box.latMin && latMax == box.latMax && lonMin ==
                box.lonMin && lonMax == box.lonMax;
        }
    };
}