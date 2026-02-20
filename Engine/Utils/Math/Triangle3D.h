#pragma once

#include "Types.h"
#include "Utils/Math/Point3D.h"

struct Triangle3D
{
	union
	{
		struct
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};

		Point3D points[3];
		float values[9];
	};

	bool IsPointInside(const Point3D& point) const;
};
