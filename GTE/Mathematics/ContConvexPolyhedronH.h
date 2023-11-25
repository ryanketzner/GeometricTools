// Addon to GeometricTools

#pragma once

#include <Mathematics/AlignedBox.h>
#include <Mathematics/ConvexPolyhedronH.h>
#include <Mathematics/ContHalfspace.h>

namespace gte
{
    template <int32_t N, typename Real>
    bool InContainer(Vector<N,Real> const& vec, ConvexPolyhedronH<N,Real> const& poly)
    {
        for (int i = 0; i < poly.halfspaces.size(); i++)
            if (!InContainer(vec, poly.halfspaces[i]))
                return false;

        return true;
    }

    template <int32_t N, typename Real>
    bool InContainer(AlignedBox<N,Real> const& box, ConvexPolyhedronH<N,Real> const& poly)
    {
        for (int i = 0; i < poly.halfspaces.size(); i++)
            if (!InContainer(box, poly.halfspaces[i]))
                return false;

        return true;
    }
}