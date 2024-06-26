
#pragma once

#include <Mathematics/PointS2.h>
#include <Mathematics/RectView3.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/ContAlignedBoxS2.h>
#include <Mathematics/ContHalfspace.h>
#include <Mathematics/MakeHalfspace.h>
#include <Mathematics/IntrRay3Sphere3.h>
#include <Mathematics/IntrRay3Ellipsoid3.h>
#include <Mathematics/IntrPlane3Sphere3.h>
#include <Mathematics/IntrPlane3Plane3.h>
#include <Mathematics/MakeHalfspace.h>
#include <Mathematics/Cone.h>
#include <Mathematics/ContRectView3Cone3.h>
#include <Mathematics/Hyperplane.h>

#include <iostream>

namespace gte
{
    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2(Vector3<Real> const& point,
        Sphere3<Real> const& sphere)
    {
        // Input point relative to sphere center
        Vector3<Real> centered(point - sphere.center);
        PointS2<Real> pointGeographic = CartToGeographic(centered);

        // Half-angle of the spherical cap defining the horizon
        Real angle = acos(sphere.radius/Length(centered));

        Real maxLat = pointGeographic.Lat() + angle;
        Real minLat = pointGeographic.Lat() - angle;
        // If contains North pole
        if (maxLat >= (Real)GTE_C_HALF_PI)
            return {minLat,GTE_C_HALF_PI,-GTE_C_PI,GTE_C_PI};
        // If contains South pole
        else if (minLat <= (Real)-GTE_C_HALF_PI)
            return {-GTE_C_HALF_PI,maxLat,-GTE_C_PI,GTE_C_PI};
        
        Real sin_angle = sin(angle);
        Real sin_colat = cos(pointGeographic.Lat());
        Real dLon = asin(sin_angle/sin_colat);

        Real maxLon = pointGeographic.Lon() + dLon;
        if (maxLon >= GTE_C_PI) maxLon -= GTE_C_TWO_PI;
        else if (maxLon <= -GTE_C_PI) maxLon += GTE_C_TWO_PI;

        Real minLon = pointGeographic.Lon() - dLon;
        if (minLon >= GTE_C_PI) minLon -= GTE_C_TWO_PI;
        else if (minLon <= -GTE_C_PI) minLon += GTE_C_TWO_PI;

        AlignedBoxS2<Real> box;
        box.Expand(minLat, pointGeographic.Lon());
        box.Expand(maxLat, pointGeographic.Lon());
        box.Expand(pointGeographic.Lat(), minLon);
        box.Expand(pointGeographic.Lat(), maxLon);

        return box;
    }

    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2(RectView3<Real> const& view, 
        Sphere3<Real> const& sphere)
    {
        // Get the rays of the rectangular view
        std::array<Ray3<Real>,4> corners;
        view.GetCorners(corners);
        const auto& halfspaces = view.halfspaces;

        // Get the projection of the corners on the sphere
        FIQuery<Real,Ray3<Real>,Sphere3<Real>> query;
        std::vector<Vector3<Real>> points;
        points.reserve(8);

        for (int i = 0; i < 4; i++)
        {
            auto result = query(corners[i],sphere);
            if (result.intersect)
                points.emplace_back(result.point[0]);
            else
                // If any ray fails to intersect, return horizon box
                return MakeFootprintBoxS2(view.vertex, sphere);
        }

        const Vector3<Real> z((int)2);
        Vector3<Real> circ_center;
        Plane3<Real> meridian_plane;
        for (int i = 0; i < 4; i++)
        {
            int i1 = i-1;
            if (i1 == -1)
                i1 = 3;

            if (halfspaces[i].constant >= sphere.radius)
                return MakeFootprintBoxS2(view.vertex, sphere);

            Real angle = acos(halfspaces[i].constant/sphere.radius);

            bool convex = false;
            if (InContainer(sphere.center, halfspaces[i]))
                circ_center = -halfspaces[i].normal*sphere.radius;
            else
            {
                convex = true;
                circ_center = halfspaces[i].normal*sphere.radius;
            }

            PointS2<Real> circ_center_geo = CartToGeographic(circ_center);            
            if (convex)
            {
                Real critical_lat = sin(circ_center_geo.Lat())/cos(angle);
                if (abs(critical_lat) <= (Real)1)
                {
                    critical_lat = asin(critical_lat);
                    Real delta_lon = asin(sin(angle)/cos(circ_center_geo.Lat()));
                    PointS2<Real> p1(critical_lat, circ_center_geo.Lon() + delta_lon, false);
                    PointS2<Real> p2(critical_lat, circ_center_geo.Lon() - delta_lon, false);
                    Vector3<Real> p1_cart = GeographicToCart(p1)*sphere.radius;
                    Vector3<Real> p2_cart = GeographicToCart(p2)*sphere.radius;

                    Vector3<Real> arc_normal = UnitCross(points[i], points[i1]);
                    Halfspace3<Real> arc_halfspace(arc_normal, (Real)0);
                    
                    if ((InContainer(p1_cart, arc_halfspace)))
                        points.emplace_back(p1_cart);

                    if ((InContainer(p2_cart, arc_halfspace)))
                        points.emplace_back(p2_cart);
                }
            }

            meridian_plane = Plane3<Real>(UnitCross(z, circ_center), (Real)0);
            if (!SameSide(meridian_plane, points[i1], points[i]))
            {   
                FIQuery<Real,Line3<Real>,Sphere3<Real>> line_query;
                FIQuery<Real,Plane3<Real>,Plane3<Real>> plane_query;
                Plane3<Real> edge_plane(halfspaces[i].normal, halfspaces[i].constant);
                auto result = line_query(plane_query(meridian_plane, edge_plane).line, sphere);
                
                if (result.intersect)
                {
                    if (Length(result.point[0] - view.vertex) < Length(result.point[1] - view.vertex))
                        points.emplace_back(result.point[0]);
                    else
                        points.emplace_back(result.point[1]);
                }
                else
                    return MakeFootprintBoxS2(view.vertex, sphere);
            }
        }

        // Get the spherical box which bounds the projected points
        AlignedBoxS2<Real> box;
        GetContainer(points,box);

        // Horizon of visible points on the sphere, as seen by the RectView3
        // origin
        Halfspace3<Real> polar = MakePolarHalfspace3(view.vertex, sphere);

        // If north pole is visibile;
        Vector3<Real> north({(Real)0, (Real)0, sphere.radius});
        if (InContainer(north, polar) && InContainer(north, view))
            box.ToNorthPolarCap();
        // If south pole is visible
        else if (InContainer(-north, polar) && InContainer(-north, view))
            box.ToSouthPolarCap();
        else if (box.IsWide())
            box.Join();

        return box;
    }

    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2_Old(RectView3<Real> const& view, 
        Sphere3<Real> const& sphere)
    {
        // Get the rays of the rectangular view
        std::array<Ray3<Real>,4> corners;
        view.GetCorners(corners);

        // Get the projection of the corners on the sphere
        FIQuery<Real,Ray3<Real>,Sphere3<Real>> query;
        std::vector<PointS2<Real>> points;
        for (int i = 0; i < 4; i++)
        {
            auto result = query(corners[i],sphere);
            if (result.intersect)
                points.emplace_back(CartToGeographic(result.point[0]));
            else
            {
                // If any ray fails to intersect, return Full box
                //return AlignedBoxS2<Real>::Full();
                return MakeFootprintBoxS2(view.vertex, sphere);
            }
        }

        // Get the spherical box which bounds the projected points
        AlignedBoxS2<Real> box;
        GetContainer(points,box);

        // Horizon of visible points on the sphere, as seen by the RectView3
        // origin
        Halfspace3<Real> polar = MakePolarHalfspace3(view.vertex, sphere);

        // If north pole is visibile;
        Vector3<Real> north({(Real)0, (Real)0, sphere.radius});
        if (InContainer(north, polar) && InContainer(north, view))
            box.ToNorthPolarCap();
        // If south pole is visible
        else if (InContainer(-north, polar) && InContainer(north, view))
            box.ToSouthPolarCap();

        return box;
    }

    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2(RectView3<Real> const& view, 
        Ellipsoid3<Real> const& ellipsoid)
    {
        // Get the rays of the rectangular view
        std::array<Ray3<Real>,4> corners;
        view.GetCorners(corners);

        // Get the projection of the corners on the sphere
        FIQuery<Real,Ray3<Real>,Ellipsoid3<Real>> query;
        std::vector<PointS2<Real>> points;
        for (int i = 0; i < 4; i++)
        {
            auto result = query(corners[i],ellipsoid);
            if (result.intersect)
                points.emplace_back(CartToGeographic(result.point[0]));
            else
            {
                // If any ray fails to intersect, return Full box
                return AlignedBoxS2<Real>::Full();
            }
        }

        // Get the spherical box which bounds the projected points
        AlignedBoxS2<Real> box;
        GetContainer(points,box);

        // Horizon of visible points on the sphere, as seen by the RectView3
        // origin
        Halfspace3<Real> polar = MakePolarHalfspace3(view.vertex, ellipsoid);

        // If north pole is visibile;
        Vector3<Real> north = ellipsoid.axis[2]*ellipsoid.extent[2];
        if (InContainer(north, polar))
            box.ToNorthPolarCap();
        // If south pole is visible
        else if (InContainer(-north, polar))
            box.ToSouthPolarCap();

        return box;
    }

    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2(Cone3<Real> const& cone, 
        Ellipsoid3<Real> const& ellipsoid)
    {
        RectView3<Real> view;
        GetContainer(cone, view);
        return MakeFootprintBoxS2(view, ellipsoid);
    }

    template <typename Real>
    AlignedBoxS2<Real> MakeFootprintBoxS2(Cone3<Real> const& cone, 
        Sphere3<Real> const& sphere)
    {
        RectView3<Real> view;
        GetContainer(cone, view);
        return MakeFootprintBoxS2(view, sphere);
    }

}