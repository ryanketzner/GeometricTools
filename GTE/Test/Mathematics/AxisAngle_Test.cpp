#include <gtest/gtest.h>
#include <iostream>

#include "Mathematics/AxisAngle.h"
#include "Mathematics/Vector3.h"
#include "Mathematics/Math.h"

using namespace gte;

#include <gtest/gtest.h>
#include <iostream>

#include "Mathematics/AxisAngle.h"
#include "Mathematics/Vector3.h"
#include "Mathematics/Math.h"

using namespace gte;

// Test the Rotate() function for AxisAngle
// Rotate the y-axis by 90 degrees about the
// x-axis. Should produce the z-axis
TEST(TestAxisAngle, TestRotate_1)
{
	Vector3<double> axis({1.0,0.0,0.0});
	double angle = GTE_C_HALF_PI;
	AxisAngle<3,double> att(axis,angle);
	
	Vector3<double> v({0.0,1.0,0.0});
	
	Vector3<double> expected_rotated({0.0,0.0,1.0});
	Vector3<double> rotated = Rotate(att,v);
	
	double tolerance = 1e-15;
	for(int i = 0; i < 3; i++)
	{
		EXPECT_NEAR(expected_rotated[i], rotated[i], tolerance);
	}
}

// Test the Rotate() function for AxisAngle
// Rotate the y-axis by -90 degrees about the
// x-axis. Should produce the -z-axis
TEST(TestAxisAngle, TestRotate_2)
{
	Vector3<double> axis({1.0,0.0,0.0});
	double angle = -GTE_C_HALF_PI;
	AxisAngle<3,double> att(axis,angle);
	
	Vector3<double> v({0.0,1.0,0.0});
	
	Vector3<double> expected_rotated({0.0,0.0,-1.0});
	Vector3<double> rotated = Rotate(att,v);
	
	double tolerance = 1e-15;
	for(int i = 0; i < 3; i++)
	{
		EXPECT_NEAR(expected_rotated[i], rotated[i], tolerance);
	}
}

// Test the Rotate() function for AxisAngle
// Rotate the x-axis by 90 degrees about the
// y-axis. Should produce the -z-axis
TEST(TestAxisAngle, TestRotate_3)
{
	Vector3<double> axis({0.0,1.0,0.0});
	double angle = GTE_C_HALF_PI;
	AxisAngle<3,double> att(axis,angle);
	
	Vector3<double> v({1.0,0.0,0.0});
	
	Vector3<double> expected_rotated({0.0,0.0,-1.0});
	Vector3<double> rotated = Rotate(att,v);
	
	double tolerance = 1e-15;
	for(int i = 0; i < 3; i++)
	{
		EXPECT_NEAR(expected_rotated[i], rotated[i], tolerance);
	}
}

// Test the Rotate() function for AxisAngle
// Rotate the x-axis by 45 degrees about the
// z-axis. Should produce the vector rotated 45 deg.
TEST(TestAxisAngle, TestRotate_4)
{
	Vector3<double> axis({0.0,0.0,1.0});
	double angle = GTE_C_QUARTER_PI;
	AxisAngle<3,double> att(axis,angle);
	
	Vector3<double> v({1.0,0.0,0.0});
	
	Vector3<double> expected_rotated({std::cos(angle),std::sin(angle),0.0});
	Vector3<double> rotated = Rotate(att,v);
	
	double tolerance = 1e-15;
	for(int i = 0; i < 3; i++)
	{
		EXPECT_NEAR(expected_rotated[i], rotated[i], tolerance);
	}
}



