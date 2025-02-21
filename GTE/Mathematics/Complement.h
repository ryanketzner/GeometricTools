// Addon to geometrictools

#pragma once

namespace gte
{
    // Represents the complement of a shape (all points not in the given shape).
    // (The ambient “universe” is assumed to be handled externally.)
    template <typename Shape>
    class Complement
    {
    public:
        explicit Complement(Shape const& s)
            : shape(s)
        {
        }

        Shape shape;
    };
}