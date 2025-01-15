// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/Hypersphere.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class SDQuery<T, Vector<N, T>, Hypersphere<N, T>>
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

        Result operator()(Vector<N,T> const& point, Hypersphere<N,T> const& hypersphere, bool robust = false)
        {
            Result result{};

			Vector<N,T> diff = point - hypersphere.center;
			result.sDistance = Length(diff, robust) - hypersphere.radius;
			
			return result;
        }
    };
}
