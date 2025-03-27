#pragma once

#include "Intersections.h"
#include <tuple>
#include <utility>

namespace gte
{
    // InContainer overload for Intersections.
    // A primitive is contained in the intersection if it is contained in every shape.
    template <typename Primitive, typename... Shapes>
    bool InContainer(Primitive const& primitive, Intersections<Shapes...> const& inter)
    {
        return std::apply(
            [&](auto const&... shape)
            {
                return (InContainer(primitive, shape) && ...);
            },
            inter.shapes
        );
    }
}