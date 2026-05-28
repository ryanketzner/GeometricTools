// Addon to GeometricTools
#pragma once

namespace gte
{
    /**
     * @brief Aligned halfspace for longitude (spherical coordinates).
     * 
     * Represents a halfspace defined by a longitude boundary on the sphere.
     * The halfspace divides the sphere along a meridian (line of constant longitude).
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class AlignedHalfspaceLon
    {
    public:
        AlignedHalfspaceLon() : 
            angle(static_cast<Real>(0)),
            dir(true)
        {
        }

        /**
         * @brief Construct a longitude halfspace.
         * 
         * @param inAngle Longitude angle in radians, range [-π, π].
         * @param inDir Direction: true = right/east (positive side), false = left/west (negative side).
         */
        AlignedHalfspaceLon(Real inAngle, bool inDir) :
            angle(inAngle),
            dir(inDir)
        {
        }

        Real angle;  ///< Longitude angle in radians, range [-π, π].
        bool dir;    ///< Direction: true = right/east, false = left/west.
    };
}

