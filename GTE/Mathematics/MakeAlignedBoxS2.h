
#pragma once

#include <Mathematics/PointS2.h>
#include <Mathematics/RectView3.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/ContAlignedBoxS2.h>
#include <Mathematics/ContHalfspace.h>
#include <Mathematics/MakeHalfspace.h>
#include <Mathematics/IntrRay3Sphere3.h>
#include <Mathematics/IntrRay3Ellipsoid3.h>
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
        
        // Calulate min and max longitudes
        // Vector3<Real> z((int)2);
        // AxisAngle<3,Real> zRot(z, angle);
        // Vector3<Real> right = Rotate(zRot, centered);
        // zRot.angle = -angle;
        // Vector3<Real> left = Rotate(zRot, centered);
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

        // box.Expand(minLat, pointGeographic.Lon());
        // box.Expand(maxLat, pointGeographic.Lon());
        // box.Expand(pointGeographic.Lat(), CartToGeographic(left).Lon());
        // box.Expand(pointGeographic.Lat(), CartToGeographic(right).Lon());

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
            {
                // If any ray fails to intersect, return Full box
                //return AlignedBoxS2<Real>::Full();
                return MakeFootprintBoxS2(view.vertex, sphere);
            }
        }

        // New Code
        FIQuery<Real,Line3<Real>,Sphere3<Real>> line_query;
        Vector3<Real> z((int)2);
        Vector3<Real> circ_center;
        for (int i = 0; i < 4; i++)
        {
            PointS2<Real> p_geo = CartToGeographic(points[i]);
            int i1 = i-1;
            if (i1 == -1)
                i1 = 3;

            // Longitude
            if (InContainer(sphere.center, halfspaces[i]))
                circ_center = -halfspaces[i].normal*sphere.radius;
            else
                circ_center = halfspaces[i].normal*sphere.radius;
            
            Plane3<Real> lat_plane(Vector3<Real>((int)2), circ_center[2]);
            Vector3<Real> normal = Cross(z, circ_center);
            Normalize(normal);
            Plane3<Real> lon_plane(normal, (Real)0);

            if (!SameSide(lat_plane, points[i1], points[i]))
            {
                normal = Cross(lat_plane.normal,halfspaces[i].normal);
                Normalize(normal);
                Line3<Real> line({view.vertex, normal});

                auto result = line_query(line,sphere);
                if (result.intersect)
                {
                    if (std::abs(result.parameter[0]) < std::abs(result.parameter[1]))
                        points.emplace_back(result.point[0]);
                    else
                        points.emplace_back(result.point[1]);
                }
                //else 
                    //std::cout << "Error." << std::endl;
            }

            if (!SameSide(lon_plane, points[i1], points[i]))
            {
                Line3<Real> line({view.vertex,
                    Cross(lon_plane.normal,halfspaces[i].normal)});

                auto result = line_query(line,sphere);
                if (result.intersect)
                {
                    if (std::abs(result.parameter[0]) < std::abs(result.parameter[1]))
                        points.emplace_back(result.point[0]);
                    else
                        points.emplace_back(result.point[1]);
                }
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
        if (InContainer(north, polar))
            box.ToNorthPolarCap();
        // If south pole is visible
        else if (InContainer(-north, polar))
            box.ToSouthPolarCap();

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
        if (InContainer(north, polar))
            box.ToNorthPolarCap();
        // If south pole is visible
        else if (InContainer(-north, polar))
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