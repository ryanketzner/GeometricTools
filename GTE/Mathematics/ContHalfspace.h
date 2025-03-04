// Addon to GeometricTools

#pragma once

#include <Mathematics/Vector.h>
#include <Mathematics/Halfspace.h>
#include <Mathematics/AlignedBox.h>

namespace gte
{
    // Test for containment.
    template <int32_t N, typename Real>
    bool InContainer(Vector<N, Real> const& point, Halfspace<N, Real> const& halfspace)
    {
        // Old Version
        return Dot(halfspace.normal,point) >= halfspace.constant;
        //return Dot(halfspace.normal,point) <= halfspace.constant;
    }

    template <int32_t N, typename Real>
    bool InContainer(AlignedBox<N, Real> const& box, Halfspace<N, Real> const& halfspace)
    {
        Real dotMin = 0;
        for (int i = 0; i < N; i++)
        {
            if (halfspace.normal[i] >= 0)
                dotMin += halfspace.normal[i] * box.min[i];
            else
                dotMin += halfspace.normal[i] * box.max[i];
        }
        
        // For the box to be entirely inside the halfspace, the minimum dot product must be >= constant.
        return dotMin >= halfspace.constant;
    }
}
