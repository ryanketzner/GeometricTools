// Addon to GeometricTools
// InContainer function which checks if any given shape is contained in the
// complement of another shape. There must be a TIQuery defined between
// the shapes, and the first shape (Primitive) must be templated on
// the Real data type used.

#pragma once

#include "Complement.h"
#include <Mathematics/TIQuery.h>
#include "Traits.h"

namespace gte
{
    // InContainer overload for the Complement shape.
    // A primitive is considered contained in the complement if it does NOT
    // intersect the underlying shape.
    template <typename Primitive, typename Shape>
    bool InContainer(Primitive const& primitive, Complement<Shape> const& comp)
    {
        // Use traits class to extract the underlying real type.
        // Need to find a way to do this that intelligently chooses type depending
        // on type of Primitive and Shape
        using Real = typename RealTraits<Primitive>::type;

        // Create the TIQuery for the underlying shape.
        TIQuery<Real, Primitive, Shape> query;
        auto result = query(primitive, comp.shape);
        
        // The primitive is contained in the complement if it does NOT intersect the shape.
        return !result.intersect;
    }
}