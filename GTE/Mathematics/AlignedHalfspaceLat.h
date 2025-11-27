// Addon to GeometricTools
#pragma once

namespace gte
{
    /**
     * @brief Aligned halfspace for latitude (spherical coordinates).
     * 
     * Represents a halfspace defined by a latitude boundary on the sphere.
     * The halfspace divides the sphere along a parallel (line of constant latitude).
     * 
     * @tparam Real Floating point type.
     */
    template <typename Real>
    class AlignedHalfspaceLat
    {
    public:
        AlignedHalfspaceLat() : 
            angle(static_cast<Real>(0)),
            dir(true)
        {
        }

        /**
         * @brief Construct a latitude halfspace.
         * 
         * @param inAngle Latitude angle in radians, range [-π/2, π/2].
         * @param inDir Direction: true = north (positive side), false = south (negative side).
         */
        AlignedHalfspaceLat(Real inAngle, bool inDir) :
            angle(inAngle),
            dir(inDir)
        {
        }

        Real angle;  ///< Latitude angle in radians, range [-π/2, π/2].
        bool dir;    ///< Direction: true = north, false = south.
    };
}

