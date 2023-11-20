// Addon to GeometricTools

#pragma once

#include <Mathematics/Hypersphere.h>
#include <Mathematics/Vector.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/DistPointAlignedBox.h>

namespace gte
{
    // Test for containment of a point inside a hypersphere.
    template <int32_t N, typename Real>
    bool InContainer(Vector<N,Real> const& point, Hypersphere<N,Real> const& sphere)
    {
        Vector<N,Real> diff = point - sphere.center;
        return Length(diff) <= sphere.radius;
    }

    // Test for containment of an AlignedBox inside a hypersphere.
    template <int32_t N, typename Real>
    bool InContainer(AlignedBox<N,Real> const& box, Hypersphere<N,Real> const& sphere)
    {
        DFPQuery<Real, Vector<N,Real>, AlignedBox<N,Real>> pbQuery;
        auto pbResult = pbQuery(sphere.center, box);
        return (pbResult.sqrDistance <= sphere.radius * sphere.radius);
    }
}