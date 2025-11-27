// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/AlignedHalfspaceLat.h>
#include <Mathematics/Math.h>
#include <cmath>

namespace gte
{
    /**
     * @brief Signed distance query from an AlignedBoxS2 to a latitude halfspace.
     * 
     * Computes the signed distance from an AlignedBoxS2 to an AlignedHalfspaceLat.
     * The distance is computed from the box's latitude boundaries to the halfspace.
     * 
     * Sign convention:
     * - Negative: box overlaps with or is inside the halfspace
     * - Zero: box is exactly on the boundary
     * - Positive: box is outside the halfspace
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>>
    {
    public:
        struct Result
        {
            Result() : sDistance(static_cast<Real>(0))
            {
            }

            Real sDistance;
        };

        Result operator()(AlignedBoxS2<Real> const& box, AlignedHalfspaceLat<Real> const& halfspace)
        {
            Result result{};
            
            Real halfspaceAngle = halfspace.angle;
            
            // Compute signed distance from box to halfspace
            // Distance is measured from the closest point on the box to the halfspace boundary
            // Negative = inside/overlapping, Positive = outside
            
            if (halfspace.dir)
            {
                // North halfspace: lat >= angle
                if (box.latMin >= halfspaceAngle)
                {
                    // Box is entirely inside: distance from latMin to angle (negative)
                    // Distance = latMin - angle, but negative because inside
                    result.sDistance = -(box.latMin - halfspaceAngle);
                }
                else if (box.latMax < halfspaceAngle)
                {
                    // Box is entirely outside: distance from latMax to angle (positive)
                    result.sDistance = halfspaceAngle - box.latMax;
                }
                else
                {
                    // Box overlaps: halfspace boundary is inside the box
                    // Return 0 when boundary is exactly on the box edge, or penetration depth
                    if (box.latMax == halfspaceAngle || box.latMin == halfspaceAngle)
                    {
                        result.sDistance = static_cast<Real>(0);
                    }
                    else
                    {
                        // Penetration depth: distance from boundary to closest point outside
                        // Since latMin < angle < latMax, closest outside point is latMin
                        result.sDistance = -(halfspaceAngle - box.latMin);
                    }
                }
            }
            else
            {
                // South halfspace: lat <= angle
                if (box.latMax <= halfspaceAngle)
                {
                    // Box is entirely inside: distance from latMax to angle (negative)
                    // Distance = angle - latMax, but negative because inside
                    result.sDistance = -(halfspaceAngle - box.latMax);
                }
                else if (box.latMin > halfspaceAngle)
                {
                    // Box is entirely outside: distance from latMin to angle (positive)
                    result.sDistance = box.latMin - halfspaceAngle;
                }
                else
                {
                    // Box overlaps: halfspace boundary is inside the box
                    // Return 0 when boundary is exactly on the box edge, or penetration depth
                    if (box.latMax == halfspaceAngle || box.latMin == halfspaceAngle)
                    {
                        result.sDistance = static_cast<Real>(0);
                    }
                    else
                    {
                        // Penetration depth: distance from boundary to closest point outside
                        // Since latMin < angle < latMax, closest outside point is latMax
                        result.sDistance = -(box.latMax - halfspaceAngle);
                    }
                }
            }
            
            return result;
        }
    };
}

