#pragma once

#include "Types.h"
#include <vector>

struct Circle3D;

struct Triangle3D
{
	union
	{
		struct
		{
			Vector3 a;
			Vector3 b;
			Vector3 c;
		};

		Vector3 points[3];
		float values[9];
	};

	bool IsPointInside(const Vector3& point) const;
	bool Raycast(const Ray& ray, OUT float& distance) const;
	bool Circlecast(const Circle3D& circle, OUT std::vector<float>& theta) const;
};
