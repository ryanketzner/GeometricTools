// Addon to geometrictools
// Defines a TIQuery for any shape and a point.
// the shape must have the incontainer function defined.
// To "intersect" a point and "contain" a point are equivalent.

#pragma once

#include <Mathematics/TIQuery.h>

namespace gte
{
    template <int32_t N, typename Real, typename Shape>
    class TIQuery<Real, Vector<N,Real>, Shape>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        Result operator()(Vector<N,Real> const& point,
                          Shape const& shape) const
        {
            return {InContainer(point,shape)};
        }
    };
}