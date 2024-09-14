
#pragma once

#include "Vector3.h"
#include "AxisAngle.h"
#include "Halfspace.h"
#include "Ray.h"

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

        RectView3()
            :
            uVector(0),
            rVector(1),
            vertex({(Real)0}),
            angleHeight((Real)GTE_C_QUARTER_PI),
            angleWidth((Real)GTE_C_QUARTER_PI)
        {
            Update();
        }

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
            {
                Normalize(halfspace.normal);
                halfspace.constant = Dot(halfspace.normal,vertex);
            }
        }

        Real GetAngleHeight() const
        {
            return angleHeight;
        }

        Real GetAngleWidth() const
        {
            return angleWidth;
        }

        // Get the corners of the RectView. These correspond to the vertices of
        // the spherical polygon representation of the RectView. The vertices
        // are given in counter-clockwise order about the boresight
        void GetCorners(std::array<Ray3<Real>,4>& corners) const
        {
            corners[0] = Ray3<Real>(vertex,
                UnitCross(halfspaces[1].normal, halfspaces[0].normal));
            corners[1] = Ray3<Real>(vertex,
                UnitCross(halfspaces[2].normal, halfspaces[1].normal));
            corners[2] = Ray3<Real>(vertex,
                UnitCross(halfspaces[3].normal, halfspaces[2].normal));
            corners[3] = Ray3<Real>(vertex,
                UnitCross(halfspaces[0].normal, halfspaces[3].normal));
        }
        
        std::array<Vector3<Real>,4> GetVertices() const
        {
			std::array<Vector3<Real>,4> vertices;
			vertices[0] = UnitCross(halfspaces[1].normal, halfspaces[0].normal);
			vertices[1] = UnitCross(halfspaces[2].normal, halfspaces[1].normal);
			vertices[2] = UnitCross(halfspaces[3].normal, halfspaces[2].normal);
			vertices[3] = UnitCross(halfspaces[0].normal, halfspaces[3].normal);
        }

        Vector3<Real> uVector, rVector, vertex;
        std::array<Halfspace3<Real>,4> halfspaces;

    protected:
        // These are protected to ensure the angle limit invariant is met
        Real angleHeight, angleWidth;
    };

}
