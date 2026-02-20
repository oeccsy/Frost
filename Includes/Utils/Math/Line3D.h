#pragma once

#include "Types.h"

struct Line3D
{
	Point3D start = Point3D(0.f);
	Point3D end = Point3D(0.f);

	float Length() { return Vector3::Distance(start, end); }
	float LengthSquared() { return Vector3::DistanceSquared(start, end); }
};