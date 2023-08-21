#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include "Mathematics/MakeAlignedBoxS2.h"
#include "Mathematics/ContAlignedBoxS2.h"
#include "Mathematics/ContRectView3.h"
#include "Mathematics/GridHypersphere.h"
#include "Mathematics/ContCone.h"

using namespace gte;

class Fixture : public ::testing::Test 
{
protected:

    // Generate a random position vector with altitude between 100 and 2000.
    void SetUp() override
    {
        double min_h = radius + 100;
        double max_h = radius + 2000;

        seed = std::random_device{}();
        std::mt19937 gen(seed);
        std::normal_distribution<double> n_dist(0.0, 1.0);
        std::uniform_real_distribution<double> u_dist(min_h, max_h);

        for (int i = 0; i < 3; i++)
            dir[i] = n_dist(gen);

        Normalize(dir);
        double h = u_dist(gen);
        pos = -dir * h;
    }

    // Static members declaration
    static unsigned int seed;
    static unsigned int grid_seed;
    static Vector3<double> pos;
    static Vector3<double> dir;
    static const double radius;
    static const Sphere3<double> sphere;
    static std::vector<Vector3<double>> grid;
};

// Static members definition and initialization
unsigned int Fixture::grid_seed = std::random_device{}();
Vector3<double> Fixture::dir;
Vector3<double> Fixture::pos;
unsigned int Fixture::seed;
const double Fixture::radius = 6378;
const Sphere3<double> Fixture::sphere({Vector3<double>::Zero(), Fixture::radius});
int num = 1000000;
std::vector<Vector3<double>> Fixture::grid = RandomSurfaceGrid(num, Fixture::sphere, Fixture::grid_seed);

TEST_F(Fixture, TestHorizon)
{
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);
    AlignedBoxS2<double> box = MakeFootprintBoxS2(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point, polar))
        {
            PointS2<double> point_geo = CartToGeographic(point);
            bool test = InContainer(CartToGeographic(point), box);


            if (!test)
            {
                std::cout << "Grid Seed: " << grid_seed << std::endl;
                std::cout << "Seed: " << seed << std::endl;
                std::cout << "Lat: " << point_geo.Lat() << std::endl;
                std::cout << "Lon: " << point_geo.Lon() << std::endl;
                std::cout << "Min: " << box.latMin << ", " << box.lonMin << std::endl;
                std::cout << "Max: " << box.latMax << ", " << box.lonMax << std::endl;
            }
            
            ASSERT_TRUE(test);
        }
    }
}

// Rect view with FOV angles generated from uniform distribution spanning .001
// to Pi - .001. View is nadir-pointing. Presumably, the orthogonal complement 
// algorithm makes roll angle of the sensor random.
TEST_F(Fixture, TestRectView3)
{
    std::mt19937 gen(seed);
    double min_angle = .001;
    double max_angle = GTE_C_PI - .001;
    std::uniform_real_distribution<double> angle_dist(min_angle, max_angle);

    std::array<Vector3<double>,2> u_r = ComputeOrthogonalComplement(dir);
    RectView3<double> view(u_r[0], u_r[1], pos, angle_dist(gen), angle_dist(gen));

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point, polar) && InContainer(point, view))
        {
            PointS2<double> point_geo = CartToGeographic(point);
            bool test = InContainer(CartToGeographic(point), box);


            if (!test)
            {
                std::cout << "Grid Seed: " << grid_seed << std::endl;
                std::cout << "Seed: " << seed << std::endl;
                std::cout << "Lat: " << point_geo.Lat() << std::endl;
                std::cout << "Lon: " << point_geo.Lon() << std::endl;
                std::cout << "Min: " << box.latMin << ", " << box.lonMin << std::endl;
                std::cout << "Max: " << box.latMax << ", " << box.lonMax << std::endl;
                std::cout << "HeightAngle: " << view.GetAngleHeight() << std::endl;
                std::cout << "WidthAngle: " << view.GetAngleWidth() << std::endl;

            }
            
            ASSERT_TRUE(test);
        }
    }
}

// Cone with FOV (full) angle generated from uniform distribution spanning .001
// to Pi/2. Cone is nadir-pointing.
TEST_F(Fixture, TestCone3)
{
    std::mt19937 gen(seed);
    double min_angle = .002;
    double max_angle = GTE_C_HALF_PI - .002;
    std::uniform_real_distribution<double> angle_dist(min_angle, max_angle);

    Ray3<double> ray(pos, dir);
    Cone3<double> cone(ray, angle_dist(gen));

    AlignedBoxS2<double> box = MakeFootprintBoxS2(cone,sphere);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point, polar) && InContainer(point, cone))
        {
            PointS2<double> point_geo = CartToGeographic(point);
            bool test = InContainer(CartToGeographic(point), box);

            if (!test)
            {
                std::cout << "Grid Seed: " << grid_seed << std::endl;
                std::cout << "Seed: " << seed << std::endl;
                std::cout << "Lat: " << point_geo.Lat() << std::endl;
                std::cout << "Lon: " << point_geo.Lon() << std::endl;
                std::cout << "Min: " << box.latMin << ", " << box.lonMin << std::endl;
                std::cout << "Max: " << box.latMax << ", " << box.lonMax << std::endl;
            }
            
            ASSERT_TRUE(test);
        }
    }
}

// Rect view with FOV angles generated from uniform distribution spanning .001
// to Pi/2 - .001. View is rotated in az/el by angle generated from uniform
// distribution spanning -Pi/2 to Pi/2.
TEST_F(Fixture, TestRectView3_Oriented)
{
    std::mt19937 gen(seed);
    double min_angle = .001;
    double max_angle = GTE_C_PI - .001;

    std::uniform_real_distribution<double> rot_dist(-GTE_C_PI/2.0, GTE_C_PI/2.0);
    std::uniform_real_distribution<double> angle_dist(min_angle, max_angle);

    std::array<Vector3<double>,2> u_r = ComputeOrthogonalComplement(dir);

    double az = rot_dist(gen);
    AxisAngle<3,double> rot_right(u_r[0], az);
    Vector3<double> r = Rotate(rot_right,u_r[1]);

    double el = rot_dist(gen);
    AxisAngle<3,double> rot_up(r, el);
    Vector3<double> u = Rotate(rot_up, u_r[0]);

    RectView3<double> view(u, r, pos, angle_dist(gen), angle_dist(gen));

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point, polar) && InContainer(point, view))
        {
            PointS2<double> point_geo = CartToGeographic(point);
            bool test = InContainer(CartToGeographic(point), box);

            if (!test)
            {
                std::cout << "Grid Seed: " << grid_seed << std::endl;
                std::cout << "Seed: " << seed << std::endl;
                std::cout << "Lat: " << point_geo.Lat() << std::endl;
                std::cout << "Lon: " << point_geo.Lon() << std::endl;
                std::cout << "Min: " << box.latMin << ", " << box.lonMin << std::endl;
                std::cout << "Max: " << box.latMax << ", " << box.lonMax << std::endl;
                std::cout << "HeightAngle: " << view.GetAngleHeight() << std::endl;
                std::cout << "WidthAngle: " << view.GetAngleWidth() << std::endl;
                std::cout << "Az: " << az << std::endl;
                std::cout << "El: " << el << std::endl;
            }
            
            ASSERT_TRUE(test);
        }
    }
}