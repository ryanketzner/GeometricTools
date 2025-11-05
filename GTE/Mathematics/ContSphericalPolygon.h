#pragma once
#include <Mathematics/SphericalPolygon.h>
#include <Mathematics/Vector3.h>

namespace gte
{
    // For this function, the query point is assumed to be specified
    // in the N frame of the spherical polygon.
    template <typename Real>
    bool InContainer(Vector3<Real> const& queryCart,SphericalPolygon<Real> const& poly)
    {
        Vector3<Real> diff = queryCart - poly.origin;
        Normalize(diff);
        return poly.Contains(diff);
    }

    // For this function, the query point is assumed to be specified
    // in the I frame of the spherical polygon.
    template <typename Real>
    bool InContainer(PointS2<Real> const& queryGeo,SphericalPolygon<Real> const& poly)
    {
        return poly.Contains(queryGeo);
    }
}
