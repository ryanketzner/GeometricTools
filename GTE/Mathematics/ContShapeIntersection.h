// Addon to GeometricTools

#pragma once

#include "Intersection.h"

namespace gte
{
    // InContainer overload for Intersection.
    // A primitive is contained in the intersection if it is contained in both shapes.
    template <typename Primitive, typename Shape1, typename Shape2>
    bool InContainer(Primitive const& primitive,
                     Intersection<Shape1, Shape2> const& inter)
    {
        return InContainer(primitive, inter.shape1) &&
               InContainer(primitive, inter.shape2);
    }
}