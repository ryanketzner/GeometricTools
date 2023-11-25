// Addon to GeometricTools

#pragma once

#include <Mathematics/Vector.h>
#include <Mathematics/Halfspace.h>

namespace gte
{
    template <int32_t N, typename Real>
    class ConvexPolyhedronH
    {
    public:
        
        // Specify N and c directly.
        ConvexPolyhedronH(std::vector<Halfspace<N,Real>> const& inHalfspaces)
            :
        halfspaces(inHalfspaces)
        {
        }

        // Public member access.
        std::vector<Halfspace<N, Real>> halfspaces;

    };

    // Template alias for convenience.
    template <typename Real>
    using ConvexPolyhedronH3 = ConvexPolyhedronH<3, Real>;

}