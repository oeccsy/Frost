#include "Plane3D.h"
#include "Point3D.h"
#include "Triangle3D.h"

bool Plane3D::IsPointInside(const Point3D& point) const
{
	float equation = normal.Dot(point) - offset;
	return fabs(equation) < FLT_EPSILON;
}

Plane3D Plane3D::FromTriangle(const Triangle3D& triangle)
{
	Plane3D plane;
	plane.normal = (triangle.b - triangle.a).Cross(triangle.c - triangle.a);
	plane.normal.Normalize();

	plane.offset = plane.normal.Dot(triangle.a);
	return plane;
}