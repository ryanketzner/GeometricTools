// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/Cone.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class SDQuery<T, Vector<N, T>, Cone<N, T>>
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

        Result operator()(Vector<N,T> const& point, Cone<N,T> const& cone, bool robust = false)
        {
            Result result{};

			Vector<N,T> diff = point - cone.ray.origin;
			T norm_diff = Length(diff, robust);
			
			T phi = acos(Dot(cone.ray.direction, diff)/norm_diff) - cone.angle;
			
			result.sDistance = norm_diff * sin(phi);
			
			return result;
        }
    };
}
