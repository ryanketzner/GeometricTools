// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedHalfspace.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class SDQuery<T, Vector<N, T>, AlignedHalfspace<N, T>>
    {
    public:
        struct Result
        {
            Result()
                :
                sDistance(static_cast<T>(0))
            {
            }

            T sDistance;
        };

        Result operator()(Vector<N,T> const& point, AlignedHalfspace<N,T> const& halfspace)
        {
        	Result result{};
        	result.sDistance = halfspace.positive ? halfspace.constant - point[halfspace.axis] : point[halfspace.axis] - halfspace.constant;
			return result;
		}
    };
}
