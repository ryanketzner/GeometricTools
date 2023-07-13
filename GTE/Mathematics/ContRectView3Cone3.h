
#pragma once

#include <Mathematics/RectView3.h>
#include <Mathematics/Cone.h>

namespace gte
{
    // Find a rectangular view which bounds a cone. There are an infinite
    // number of possible solutions, since an infinite number of orthogonal
    // bases can be created from a single vector. The solution is guaranteed
    // to be a tight fit to the cone
    template <typename Real>
    bool GetContainer(Cone3<Real> const& cone, RectView3<Real>& view)
    {
        Real angle = cone.angle*(Real)2;
        view.SetAngleHeight(angle);
        view.SetAngleWidth(angle);
        view.vertex = cone.ray.origin;

        std::array<Vector3<Real>,2> upRight = 
            ComputeOrthogonalComplement(cone.ray.direction);

        view.uVector = upRight[0];
        view.rVector = upRight[1];
        view.Update();

        return true;
    }
}