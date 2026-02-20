#pragma once

#include "Types.h"

struct Plane3D
{
	Vector3 normal;
	float offset;

	bool IsPointInside(const struct Point3D& point) const;
	static Plane3D FromTriangle(const struct Triangle3D& triangle);
};