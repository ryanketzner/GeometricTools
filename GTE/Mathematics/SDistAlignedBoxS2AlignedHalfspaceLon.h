// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/AlignedHalfspaceLon.h>
#include <Mathematics/PointS2.h>
#include <Mathematics/Math.h>
#include <cmath>

namespace gte
{
    /**
     * @brief Signed distance query from a point on the sphere to a longitude halfspace.
     * 
     * Computes the signed distance from a PointS2 to an AlignedHalfspaceLon.
     * The distance is the angular difference in longitude, taking the shorter
     * path around the sphere (handling wrap-around at ±π).
     * 
     * Sign convention:
     * - Negative: point is inside the halfspace (on the correct side)
     * - Zero: point is exactly on the boundary
     * - Positive: point is outside the halfspace
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class SDQuery<Real, PointS2<Real>, AlignedHalfspaceLon<Real>>
    {
    public:
        struct Result
        {
            Result() : sDistance(static_cast<Real>(0))
            {
            }

            Real sDistance;
        };

        Result operator()(PointS2<Real> const& point, AlignedHalfspaceLon<Real> const& halfspace)
        {
            Result result{};
            
            Real pointLon = point.Lon();
            Real halfspaceAngle = halfspace.angle;
            
            // Compute angular difference, handling wrap-around
            Real diff = pointLon - halfspaceAngle;
            
            // Normalize to [-π, π] range (pick shorter path)
            if (diff > GTE_C_PI)
                diff -= GTE_C_TWO_PI;
            else if (diff < -GTE_C_PI)
                diff += GTE_C_TWO_PI;
            
            // Apply sign based on direction
            // If dir is true (right/east), the halfspace is "longitude >= angle"
            // If dir is false (left/west), the halfspace is "longitude <= angle"
            if (halfspace.dir)
            {
                // Right/east halfspace: inside if pointLon >= angle (diff >= 0)
                // Distance is negative when inside, positive when outside
                result.sDistance = -diff;  // Negative when inside (diff >= 0)
            }
            else
            {
                // Left/west halfspace: inside if pointLon <= angle (diff <= 0)
                // Distance is negative when inside, positive when outside
                result.sDistance = diff;   // Negative when inside (diff <= 0)
            }
            
            return result;
        }
    };
}

