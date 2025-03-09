#pragma once
#include "Types.h"

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;
};

struct TransformData
{
	Matrix worldMatrix = DirectX::XMMatrixIdentity();
	Matrix viewMatrix = DirectX::XMMatrixIdentity();
	Matrix projMatrix = DirectX::XMMatrixIdentity();
};

using Point3D = Vector3;

struct Line3D
{
	Point3D start = Point3D(0.f);
	Point3D end = Point3D(0.f);

	float Length() { return Vector3::Distance(start, end); }
	float LengthSq() { return Vector3::DistanceSquared(start, end); }
};

struct Sphere3D
{
	Point3D position;
	float radius;
};

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

struct OBB3D
{
	Point3D position = Vector3(0.f);
	Vector3 size = Vector3(1.f, 1.f, 1.f);
	Matrix orientation;
};

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
};

struct Plane3D
{
	Vector3 normal;
	float offset;

	static Plane3D FromTriangle(const Triangle3D& triangle)
	{
		Plane3D plane;
		plane.normal = (triangle.b - triangle.a).Cross(triangle.c - triangle.a);
		plane.normal.Normalize();

		plane.offset = plane.normal.Dot(triangle.a);
		return plane;
	}
};