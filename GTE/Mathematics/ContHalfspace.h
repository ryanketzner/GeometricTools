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
        return Dot(halfspace.normal,point) >= halfspace.constant;
    }

    template <int32_t N, typename Real>
    bool InContainer(AlignedBox<N, Real> const& box, Halfspace<N, Real> const& halfspace)
    {
        constexpr int32_t arraySize = std::pow(2, N);
        std::array<Vector<N,Real>, arraySize> vertices;
        box.GetVertices(vertices);

        for (int i = 1; i < vertices.size() - 1; i++)
        {
            // If any vertex is not in the container,
            // immediately terminate
            if (InContainer(vertices[i], halfspace))
                return false;
        }

        // All vertices are in container
        return true;
    }
}
