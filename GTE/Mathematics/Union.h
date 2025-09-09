// Addon to geometrictools

#pragma once

namespace gte
{
    // Represents the region common to two shapes.
    template <typename Shape1, typename Shape2>
    class Union
    {
    public:
        Union(Shape1 const& s1, Shape2 const& s2)
            : shape1(s1), shape2(s2)
        {
        }

        Shape1 shape1;
        Shape2 shape2;
    };
}
