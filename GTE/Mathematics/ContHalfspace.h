// Addon to GeometricTools

#pragma once

#include <Mathematics/Vector.h>
#include <Mathematics/Halfspace.h>

namespace gte
{
    // Test for containment.
    template <int32_t N, typename Real>
    bool InContainer(Vector<N, Real> const& point, Halfspace<N, Real> const& halfspace)
    {
        return Dot(halfspace.normal,point) >= halfspace.constant;
    }
}
