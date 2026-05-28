// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/AlignedHalfspaceLon.h>
#include <Mathematics/Math.h>
#include <cmath>

namespace gte
{
    /**
     * @brief Signed distance query from an AlignedBoxS2 to a longitude halfspace.
     * 
     * Computes the signed distance from an AlignedBoxS2 to an AlignedHalfspaceLon.
     * The distance is computed from the box's longitude boundaries to the halfspace.
     * 
     * Sign convention:
     * - Negative: box overlaps with or is inside the halfspace
     * - Zero: box is exactly on the boundary
     * - Positive: box is outside the halfspace
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>>
    {
    public:
        struct Result
        {
            Result() : sDistance(static_cast<Real>(0))
            {
            }

            Real sDistance;
        };

        Result operator()(AlignedBoxS2<Real> const& box, AlignedHalfspaceLon<Real> const& halfspace)
        {
            Result result{};
            
            Real halfspaceAngle = halfspace.angle;
            
            // Handle empty box
            if (box.IsEmptyLon())
            {
                result.sDistance = static_cast<Real>(GTE_C_PI);  // Far outside
                return result;
            }
            
            // If box is wide (spans >= 180°), it covers all longitudes - always overlaps
            if (box.IsWide())
            {
                result.sDistance = static_cast<Real>(-GTE_C_PI);  // Inside (overlaps)
                return result;
            }
            
            // Helper function to compute shortest angular distance (always returns positive or zero)
            auto shortestAngularDist = [](Real a, Real b) -> Real {
                Real dist = a - b;
                // Normalize to [-π, π]
                while (dist > GTE_C_PI) dist -= GTE_C_TWO_PI;
                while (dist < -GTE_C_PI) dist += GTE_C_TWO_PI;
                return std::abs(dist);
            };
            
            // Check if box wraps around antimeridian (lonMin > lonMax)
            bool isWrapped = box.IsInverted();
            
            if (halfspace.dir)
            {
                // East halfspace: lon >= angle
                if (isWrapped)
                {
                    // Wrapped box spans two intervals: [lonMin, π] and [-π, lonMax]
                    // Check if halfspace boundary is inside either interval
                    bool inFirstInterval = (halfspaceAngle >= box.lonMin && halfspaceAngle <= GTE_C_PI);
                    bool inSecondInterval = (halfspaceAngle >= -GTE_C_PI && halfspaceAngle <= box.lonMax);
                    
                    if (inFirstInterval || inSecondInterval)
                    {
                        // Box overlaps with halfspace - return negative (penetration depth)
                        // Compute minimum distance from box boundaries to halfspace
                        Real dist1 = shortestAngularDist(box.lonMin, halfspaceAngle);
                        Real dist2 = shortestAngularDist(box.lonMax, halfspaceAngle);
                        Real minDist = std::min(std::abs(dist1), std::abs(dist2));
                        result.sDistance = -minDist;
                    }
                    else
                    {
                        // Box is outside - compute shortest distance from box to boundary
                        // Check both intervals
                        Real dist1 = shortestAngularDist(halfspaceAngle, box.lonMin);
                        Real dist2 = shortestAngularDist(halfspaceAngle, box.lonMax);
                        Real dist3 = shortestAngularDist(halfspaceAngle, GTE_C_PI);  // End of first interval
                        Real dist4 = shortestAngularDist(halfspaceAngle, -GTE_C_PI);  // Start of second interval
                        Real minDist = std::min({std::abs(dist1), std::abs(dist2), std::abs(dist3), std::abs(dist4)});
                        result.sDistance = minDist;
                    }
                }
                else
                {
                    // Normal box: lonMin <= lonMax
                    // For east halfspace (lon >= angle), compute distance from halfspace to closest point of box
                    
                    // Find closest point of box to halfspace (considering wrap-around)
                    Real distToMin = shortestAngularDist(box.lonMin, halfspaceAngle);
                    Real distToMax = shortestAngularDist(box.lonMax, halfspaceAngle);
                    Real minDist = std::min(distToMin, distToMax);
                    Real closestPoint = (distToMin < distToMax) ? box.lonMin : box.lonMax;
                    
                    // Check if box is inside (closest point >= angle, considering wrap-around)
                    bool isInside = false;
                    // Direct comparison
                    if (closestPoint >= halfspaceAngle)
                    {
                        isInside = true;
                    }
                    // Wrap-around: if closest point is near π and halfspace is near -π
                    // Check if going the short way, closest point >= halfspace
                    else if (closestPoint > 0 && halfspaceAngle < 0)
                    {
                        // The shortest path goes through antimeridian
                        // If minDist < π/2, then going forward from halfspace reaches closest point
                        // which means closest point is "ahead" of halfspace, so inside
                        if (minDist < GTE_C_PI)
                        {
                            isInside = true;
                        }
                    }
                    
                    if (isInside)
                    {
                        result.sDistance = -minDist;
                    }
                    else if (box.lonMax < halfspaceAngle && box.lonMin < halfspaceAngle)
                    {
                        // Box is entirely outside (both endpoints < angle, no wrap-around case)
                        result.sDistance = minDist;
                    }
                    else
                    {
                        // Box overlaps: halfspace boundary is inside the box
                        result.sDistance = static_cast<Real>(0);
                    }
                }
            }
            else
            {
                // West halfspace: lon <= angle
                if (isWrapped)
                {
                    // Wrapped box spans two intervals: [lonMin, π] and [-π, lonMax]
                    // Check if halfspace boundary is inside either interval
                    bool inFirstInterval = (halfspaceAngle >= box.lonMin && halfspaceAngle <= GTE_C_PI);
                    bool inSecondInterval = (halfspaceAngle >= -GTE_C_PI && halfspaceAngle <= box.lonMax);
                    
                    if (inFirstInterval || inSecondInterval)
                    {
                        // Box overlaps with halfspace - return negative (penetration depth)
                        Real dist1 = shortestAngularDist(halfspaceAngle, box.lonMin);
                        Real dist2 = shortestAngularDist(halfspaceAngle, box.lonMax);
                        Real minDist = std::min(std::abs(dist1), std::abs(dist2));
                        result.sDistance = -minDist;
                    }
                    else
                    {
                        // Box is outside - compute shortest distance
                        Real dist1 = shortestAngularDist(box.lonMin, halfspaceAngle);
                        Real dist2 = shortestAngularDist(box.lonMax, halfspaceAngle);
                        Real dist3 = shortestAngularDist(GTE_C_PI, halfspaceAngle);
                        Real dist4 = shortestAngularDist(-GTE_C_PI, halfspaceAngle);
                        Real minDist = std::min({std::abs(dist1), std::abs(dist2), std::abs(dist3), std::abs(dist4)});
                        result.sDistance = minDist;
                    }
                }
                else
                {
                    // Normal box: lonMin <= lonMax
                    if (box.lonMax <= halfspaceAngle)
                    {
                        // Box is entirely inside
                        Real dist = shortestAngularDist(halfspaceAngle, box.lonMax);
                        result.sDistance = -std::abs(dist);
                    }
                    else if (box.lonMin > halfspaceAngle)
                    {
                        // Box is entirely outside
                        Real dist = shortestAngularDist(box.lonMin, halfspaceAngle);
                        result.sDistance = std::abs(dist);
                    }
                    else
                    {
                        // Box overlaps: halfspace boundary is inside the box
                        result.sDistance = static_cast<Real>(0);
                    }
                }
            }
            
            return result;
        }
    };
}

