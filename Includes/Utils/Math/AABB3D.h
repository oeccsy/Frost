#pragam once

#include "Types.h"

struct AABB3D
{
	Point3D position = Vector3(0.f);
	Vector3 size = Vector3(1.f, 1.f, 1.f);

	static Vector3 GetMin(const AABB3D& aabb)
	{
		Vector3 p1 = aabb.position + aabb.size;
		Vector3 p2 = aabb.position - aabb.size;
		
		return Vector3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	static Vector3 GetMax(const AABB3D& aabb)
	{
		Vector3 p1 = aabb.position + aabb.size;
		Vector3 p2 = aabb.position - aabb.size;

		return Vector3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	static AABB3D FromMinMax(const Vector3& min, const Vector3& max)
	{
		return AABB3D{ (min + max) / 2, (max - min) / 2 };
	}
};