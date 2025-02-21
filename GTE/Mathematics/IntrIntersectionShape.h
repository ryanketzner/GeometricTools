// Addon to geometrictools

#pragma once

#include <Mathematics/TIQuery.h>
#include <Mathematics/Intersection.h>

namespace gte
{
    // TIQuery specialization for Intersection.
    // This works for any primitive type 'Primitive' as long as
    // TIQuery<Real, Primitive, Shape1> and TIQuery<Real, Primitive, Shape2> are defined.
    template <typename Real, typename Primitive, typename Shape1, typename Shape2>
    class TIQuery<Real, Primitive, Intersection<Shape1, Shape2>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        Result operator()(Primitive const& primitive,
                          Intersection<Shape1, Shape2> const& inter) const
        {
            // Query each component shape.
            TIQuery<Real, Primitive, Shape1> query1;
            TIQuery<Real, Primitive, Shape2> query2;

            // The primitive intersects the intersection if it intersects both shapes.
            return {query1(primitive, inter.shape1).intersect && query2(primitive, inter.shape2).intersect};
        }
    };

}