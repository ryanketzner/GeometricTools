#pragma once

#include <tuple>
#include <array>
#include <utility>

namespace gte
{
    // Represents the region common to an arbitrary number of shapes.
    template <typename... Shapes>
    class Intersections
    {
    public:
        // Constructor that accepts an arbitrary number of shapes.
        explicit Intersections(Shapes const&... shapes)
            : shapes(std::make_tuple(shapes...))
        {
        }

        // Constructor that accepts a tuple of shapes.
        explicit Intersections(std::tuple<Shapes...> const& tup)
            : shapes(tup)
        {
        }

        std::tuple<Shapes...> shapes;
    };

    // Deduction guide for constructing an Intersections object from a tuple.
    template <typename... Shapes>
    Intersections(std::tuple<Shapes...> const&) -> Intersections<Shapes...>;
}