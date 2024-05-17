// Addon to geometrictools

#pragma once

#include <Mathematics/FIQuery.h>
#include <Mathematics/TIQuery.h>
#include <Mathematics/AlignedBox.h>

// The queries consider the box to be a solid.
//
// The aligned-aligned queries use simple min-max comparisions.  The
// interesection of aligned boxes is an aligned box, possibly degenerate,
// where min[d] == max[d] for at least one dimension d.

namespace gte
{
    template <int32_t N, typename T>
    class TIQuery<T, AlignedBox<N,T>, AlignedBox<N,T>>
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

        Result operator()(AlignedBox<N,T> const& box0, AlignedBox<N,T> const& box1)
        {
            Result result{};
            for (int32_t i = 0; i < N; i++)
            {
                if (box0.max[i] < box1.min[i] || box0.min[i] > box1.max[i])
                {
                    result.intersect = false;
                    return result;
                }
            }
            result.intersect = true;
            return result;
        }
    };
}
