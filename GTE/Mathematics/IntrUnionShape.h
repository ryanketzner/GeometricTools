// Addon to geometrictools

#pragma once

#include <Mathematics/TIQuery.h>
#include <Mathematics/Union.h>

namespace gte
{
    // TIQuery specialization for Union.
    // This works for any primitive type 'Primitive' as long as
    // TIQuery<Real, Primitive, Shape1> and TIQuery<Real, Primitive, Shape2> are defined.
    template <typename Real, typename Primitive, typename Shape1, typename Shape2>
    class TIQuery<Real, Primitive, Union<Shape1, Shape2>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        Result operator()(Primitive const& primitive,
                          Union<Shape1, Shape2> const& uni) const
        {
            // Query each component shape.
            TIQuery<Real, Primitive, Shape1> query1;
            TIQuery<Real, Primitive, Shape2> query2;

            // The primitive intersects the union if it intersects either shape.
            return {query1(primitive, uni.shape1).intersect || query2(primitive, uni.shape2).intersect};
        }
    };

}
