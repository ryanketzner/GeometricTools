#pragma once

#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

#include <Mathematics/TIQuery.h>
#include <Mathematics/Intersections.h>

namespace gte
{
    template <typename Real, typename Primitive, typename... Shapes>
    class TIQuery<Real, Primitive, Intersections<Shapes...>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        // Default-construct a tuple of individual TIQuery objects (one per shape).
        // (Mark queries_ mutable so non-const operator() calls can be made.)
        TIQuery() : queries_{} {}

        Result operator()(Primitive const& primitive,
                          Intersections<Shapes...> const& inter)
        {
            // Use a templated lambda to generate an index sequence inline.
            bool allIntersect = [&]<std::size_t... I>(std::index_sequence<I...>)
            {
                return ( ... && ( std::get<I>(queries_)(primitive, std::get<I>(inter.shapes)).intersect ) );
            }(std::make_index_sequence<sizeof...(Shapes)>{});
            return { allIntersect };
        }

    private:
        std::tuple<TIQuery<Real, Primitive, Shapes>...> queries_;
    };
}