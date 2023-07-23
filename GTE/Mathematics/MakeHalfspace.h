// Addon to GeometricTools

#pragma once

#include <Mathematics/Vector3.h>
#include <Mathematics/Halfspace.h>
#include <Mathematics/Hypersphere.h>
#include <Mathematics/Hyperellipsoid.h>

namespace gte
{
    template <typename Real>
    Halfspace3<Real> MakePolarHalfspace3(Vector3<Real> const& point,
        Sphere3<Real> const& sphere)
    {
        return {point - sphere.center, sphere.radius*sphere.radius};
    }

    // Equation (38) from "Surface Area of an Elipsoid Segment", Garry Tee
    template <typename Real>
    Halfspace3<Real> MakePolarHalfspace3(Vector3<Real> const& point,
        Ellipsoid3<Real> const& ellipsoid)
    {
        return {(point - ellipsoid.center)/(ellipsoid.extent*ellipsoid.extent),
            (Real)1};
    }
}