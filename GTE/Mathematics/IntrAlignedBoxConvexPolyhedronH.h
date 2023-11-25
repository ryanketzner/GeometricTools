// Addon to GeometricTools

#pragma once

#include <Mathematics/AlignedBox.h>
#include <Mathematics/ConvexPolyhedronH.h>
#include <Mathematics/IntrAlignedBoxHalfspace.h>
#include <Mathematics/TIQuery.h>

namespace gte
{
    template <int32_t N, typename T>
    class TIQuery<T, AlignedBox<N,T>, ConvexPolyhedronH<N,T>>
    {
    public:

        struct Result
        {
            Result()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Result operator()(AlignedBox<N,T> const& box, ConvexPolyhedronH<N,T> const& poly)
        {
            TIQuery<T, AlignedBox<N,T>, Halfspace<N,T>> query;

            Result result{};

            for (int i = 0; i < poly.halfspaces.size(); i++)
                if (!query(box, poly.halfspaces[i]).intersect)
                {
                    result.intersect = false;
                    return result;
                }

            result.intersect = true;
            return result;
        }
    };
}