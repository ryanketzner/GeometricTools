// Addon to geometrictools

#pragma once

#include <Mathematics/TIQuery.h>
#include <Mathematics/Complement.h>
#include <Mathematics/ContShapeComplement.h>

namespace gte
{
    template <typename Real, typename Primitive, typename Shape>
    class TIQuery<Real, Primitive, Complement<Shape>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        Result operator()(Primitive const& primitive,
                          Complement<Shape> const& inter) const
        {
            // The primitive intersects the complement if it isn't contained by the shape.
            return {!InContainer(primitive, inter.shape)};
        }
    };

}
