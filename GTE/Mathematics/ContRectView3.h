// Addon to GeometricTools

#pragma once

#include <Mathematics/RectView3.h>
#include <Mathematics/Vector3.h>
#include "Mathematics/Halfspace.h"
#include "Mathematics/ContHalfspace.h"

namespace gte
{
    template <typename Real>
    bool InContainer(Vector3<Real> const& point, RectView3<Real> const& view)
    {
        for (const auto& halfspace : view.halfspaces)
            if (!InContainer(point,halfspace))
                return false;

        return true;
    }
}

