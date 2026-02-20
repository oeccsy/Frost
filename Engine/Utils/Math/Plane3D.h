#pragma once

#include "Types.h"

struct Plane3D
{
	Vector3 normal;
	float offset;

	bool IsPointInside(const Vector3& point) const;
	bool Raycast(const Ray& ray, OUT float& distance) const;
	static Plane3D FromTriangle(const struct Triangle3D& triangle);
};