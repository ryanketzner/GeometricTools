// Addon to GeometricTools

#pragma once

#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/PointS2.h>

namespace gte
{
    template <typename Real>
    bool InContainer(PointS2<Real> const& point, AlignedBoxS2<Real> const& box)
    {
        return box.ContainsLat(point.Lat()) && box.ContainsLon(point.Lon());
    }

    // Construct an aligned box on the sphere that contains the input points.
    // The result is not necessarily the minimum size box.
    // A version of this function which computes the minimum sized box could be
    // made using the linear-time solution to the "Maximum Gap Problem."
    template <typename Real> 
    bool GetContainer(std::vector<PointS2<Real>> const& points, AlignedBoxS2<Real>& box)
    {
        for (int i = 0; i < points.size(); i++)
            box.Expand(points[i].Lat(),points[i].Lon());
        
        return true;
    }

    template <typename Real> 
    bool GetContainer(std::vector<Vector3<Real>> const& points, AlignedBoxS2<Real>& box)
    {
        for (int i = 0; i < points.size(); i++)
        {
            PointS2<Real> point = CartToGeographic(points[i]);
            box.Expand(point.Lat(),point.Lon());
        }
        
        return true;
    }

}