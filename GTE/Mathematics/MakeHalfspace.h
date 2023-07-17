
#pragma once

#include <Mathematics/Halfspace.h>
#include <Mathematics/Hypersphere.h>

namespace gte
{
    template <typename Real>
    Halfspace3<Real> MakePolarHalfspace3(Vector3<Real> const& point,
        Sphere3<Real> const& sphere)
    {
        return {point - sphere.center, sphere.radius*sphere.radius};
    }
}