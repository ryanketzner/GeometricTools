// Addon to GeometricTools

#pragma once

#include <Mathematics/DistPointAlignedBox.h>
#include <Mathematics/Hypersphere.h>

namespace gte
{
    template <int32_t N, typename T>
    class TIQuery<T, AlignedBox<N,T>, Hypersphere<N,T>>
    {
    public:
        // The intersection query considers the box and sphere to be solids;
        // that is, the sphere object includes the region inside the spherical
        // boundary and the box object includes the region inside the cuboid
        // boundary.  If the sphere object and box object object overlap, the
        // objects intersect.
        struct Result
        {
            Result()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Result operator()(AlignedBox<N,T> const& box, Hyperphere<N,T> const& sphere)
        {
            DCPQuery<T, Vector<N,T>, AlignedBox<N,T>> pbQuery;
            auto pbResult = pbQuery(sphere.center, box);
            Result result{};
            result.intersect = (pbResult.sqrDistance <= sphere.radius * sphere.radius);
            return result;
        }
    };
}