#pragma once

#include "Types.h"

struct OBB3D
{
	Point3D position = Vector3(0.f);
	Vector3 size = Vector3(1.f, 1.f, 1.f);
	Matrix orientation;
};