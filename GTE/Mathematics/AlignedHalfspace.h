// Addon to GeometricTools

#pragma once

template <int32_t N, typename T>
class AlignedHalfspace
{

public:

	AlignedHalfspace() : 
	axis(0),
	positive(true),
	constant(0.0)
	{}

	AlignedHalfspace(int axis, bool positive, T constant) :
	axis(axis),
	positive(positive),
	constant(constant)
	{
	}
	
	int axis;
	bool positive;
	T constant;
};
