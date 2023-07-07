
#pragma once

#include "Vector3.h"
#include "AxisAngle.h"
#include "Halfspace.h"
#include <Mathematics/Logger.h>

// A rectangular field of view. The field of view orientation is represented
// by an up vector and a right vector, where the sensor boresight points along
// Cross(up,right). The heightAngle is the angle subtended by the top and 
// bottom planes. The widthAngle is the angle subtended by the right and left
// planes. The field of view must not extend beyond one hemisphere, so the
// angles are limited to be between zero and Pi.

namespace gte
{
    template <typename Real>
    class RectView3
    {
    public:

        RectView3(Vector3<Real> const& inUVector, 
            Vector3<Real> const& inRVector, Vector3<Real> const& inVertex,
            Real inAngleHeight, Real inAngleWidth)
            :
            uVector(inUVector),
            rVector(inRVector),
            vertex(inVertex)
        {
            SetAngleHeight(inAngleHeight);
            SetAngleWidth(inAngleWidth);
            Update();
        }

        void SetAngleHeight(Real inAngleHeight)
        {
            LogAssert((Real)0 < inAngleHeight && inAngleHeight < (Real)GTE_C_PI, "Invalid angle.");
            angleHeight = inAngleHeight;
        }

        void SetAngleWidth(Real inAngleWidth)
        {
            LogAssert((Real)0 < inAngleWidth && inAngleWidth < (Real)GTE_C_PI, "Invalid angle.");
            angleWidth = inAngleWidth;
        }

        // Updates the halfspaces, which are needed for fast point containment
        // Update must be called after any member is updated, whether through
        // a setter or public access.
        void Update()
        {
            // Get first normal by rotating -r clockwise about u
            AxisAngle<3,Real> r1(uVector,-angleWidth/(Real)2);
            halfspaces[0].normal = Rotate(r1,-rVector);

            // Get second normal by rotating -u counterclockwise about r
            AxisAngle<3,Real> r2(rVector,angleHeight/(Real)2);
            halfspaces[1].normal = Rotate(r2,-uVector);

            // Get third normal by rotating r counterclockwise about u
            AxisAngle<3,Real> r3(uVector,angleWidth/(Real)2);
            halfspaces[2].normal = Rotate(r3,rVector);

            // Get fourth normal by rotating u clockwise about r
            AxisAngle<3,Real> r4(rVector,-angleHeight/(Real)2);
            halfspaces[3].normal = Rotate(r4,uVector);

            for (auto& halfspace : halfspaces)
                halfspace.constant = Dot(halfspace.normal,vertex);
        }

        Vector3<Real> uVector, rVector, vertex;
        std::array<Halfspace3<Real>,4> halfspaces;

    protected:
        // These are protected to ensure the angle limit invariant is met
        Real angleHeight, angleWidth;
    };

}