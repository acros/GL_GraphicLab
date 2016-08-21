#pragma once

#include "esUtil.h"

typedef ESMatrix AcMatrix;

struct AcVector {
	AcVector() :x(0.f), y(0.f), z(0.f)
	{}

	AcVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{}

	float	x, y, z;

	static AcVector	ONE;
};