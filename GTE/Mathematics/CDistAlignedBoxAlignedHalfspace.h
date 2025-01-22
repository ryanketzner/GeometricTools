// Addon to GeometricTools
#pragma once

#include <Mathematics/CDQuery.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/AlignedHalfspace.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class CDQuery<T, AlignedBox<N, T>, AlignedHalfspace<N, T>>
    {
    public:
        struct Result
        {
            Result()
                :
                cDistance(static_cast<T>(0))
            {
            }

            T cDistance;
        };

        Result operator()(AlignedBox<N,T> const& box, AlignedHalfspace<N,T> const& halfspace)
        {
        	Result result{};
        	T width = box.max[halfspace.axis] - box.min[halfspace.axis];
        	result.cDistance = halfspace.positive ? halfspace.constant - box.max[halfspace.axis] - width : box.min[halfspace.axis] - halfspace.constant - width;
			return result;
		}
    };
}
