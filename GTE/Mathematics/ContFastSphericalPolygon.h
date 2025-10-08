#pragma once
#include <Mathematics/FastSphericalPolygon.h>

namespace gte
{
    template <typename Real>
    bool InContainer(FastSphericalPolygon<Real> const& poly, Vector3<Real> const& queryCart)
    {
        return poly.Contains(queryCart);
    }

    template <typename Real>
    bool InContainer(FastSphericalPolygon<Real> const& poly, PointS2<Real> const& queryGeo)
    {
        return poly.Contains(queryGeo);
    }
}