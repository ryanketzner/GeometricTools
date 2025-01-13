// Addon to GeometricTools

#include <Mathematics/SDQuery.h>
#include <Mathematics/Halfspace.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class SDQuery<T, Vector<N, T>, Halfspace<N, T>>
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

        Result operator()(Vector<N,T> const& point, Halfspace<N,T> const& halfspace, bool robust = false)
        {
            Result result{};

			result.sDistance = halfspace.constant - Dot(point, halfspace.normal);
			
			return result;
        }
    };
}
