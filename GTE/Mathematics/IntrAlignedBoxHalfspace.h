// Addon to GeometricTools

#pragma once

#include <Mathematics/AlignedBox.h>
#include <Mathematics/Halfspace.h>
#include <Mathematics/TIQuery.h>

namespace gte
{
    template <int32_t N, typename T>
    class TIQuery<T, AlignedBox<N,T>, Halfspace<N,T>>
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

        Result operator()(AlignedBox<N,T> const& box, Halfspace<N,T> const& halfspace)
        {
            T dot = 0;
            for (int i = 0; i < N; i++)
            {
                if (halfspace.normal[i] <= 0)
                    dot += halfspace.normal[i] * box.min[i];
                else
                    dot += halfspace.normal[i] * box.max[i];
            }

            Result result{};

            if (dot >= halfspace.constant)
                result.intersect = true;
            else
                result.intersect = false;

            return result;
        }
    };
}
