// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/AlignedHalfspaceLat.h>
#include <Mathematics/PointS2.h>
#include <Mathematics/Math.h>
#include <cmath>

namespace gte
{
    /**
     * @brief Signed distance query from a point on the sphere to a latitude halfspace.
     * 
     * Computes the signed distance from a PointS2 to an AlignedHalfspaceLat.
     * The distance is simply the difference in latitude.
     * 
     * Sign convention:
     * - Negative: point is inside the halfspace (on the correct side)
     * - Zero: point is exactly on the boundary
     * - Positive: point is outside the halfspace
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class SDQuery<Real, PointS2<Real>, AlignedHalfspaceLat<Real>>
    {
    public:
        struct Result
        {
            Result() : sDistance(static_cast<Real>(0))
            {
            }

            Real sDistance;
        };

        Result operator()(PointS2<Real> const& point, AlignedHalfspaceLat<Real> const& halfspace)
        {
            Result result{};
            
            Real pointLat = point.Lat();
            Real halfspaceAngle = halfspace.angle;
            
            // Compute difference in latitude
            Real diff = pointLat - halfspaceAngle;
            
            // Apply sign based on direction
            // If dir is true (north), positive diff means we're to the north (inside)
            // If dir is false (south), negative diff means we're to the south (inside)
            if (halfspace.dir)
            {
                // North halfspace: inside if diff >= 0
                result.sDistance = -diff;  // Negative when inside (diff >= 0)
            }
            else
            {
                // South halfspace: inside if diff <= 0
                result.sDistance = diff;   // Negative when inside (diff <= 0)
            }
            
            return result;
        }
    };
}

