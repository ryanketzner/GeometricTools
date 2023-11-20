// Addon to GeometricTools

#pragma once

#include <Mathematics/Math.h>

namespace gte
{
    // Find-bounding box queries.

    template <typename Real, typename Type0, typename Type1>
    class FBQuery
    {
    public:
        struct Result
        {
            // A FBQuery-base class B must define a B::Result struct with
            // member 'bool intersect'.  A FBQuery-derived class D must also
            // derive a D::Result from B:Result but may have no members.  The
            // member 'intersect' is 'true' iff the primitives intersect.  The
            // operator() is const for conceptual constness, but derived
            // classes can use internal data to support the queries and tag
            // that data with the mutable modifier.
        };

        Result operator()(Type0 const& primitive0, Type1 const& primitive1) const;
    };
}