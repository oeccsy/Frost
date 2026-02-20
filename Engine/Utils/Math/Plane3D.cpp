#include "Plane3D.h"
#include "Triangle3D.h"

bool Plane3D::IsPointInside(const Vector3& point) const
{
	float equation = normal.Dot(point) - offset;
	return fabs(equation) < FLT_EPSILON;
}

bool Plane3D::Raycast(const Ray& ray, OUT float& distance) const
{
	float n_dot_o = normal.Dot(ray.position);
	float n_dot_d = normal.Dot(ray.direction);
	if (n_dot_d < FLT_EPSILON) return false;

	float t = -(n_dot_o - offset) / n_dot_d;
	if (t < 0) return false;

	distance = t;

	return true;
}

Plane3D Plane3D::FromTriangle(const Triangle3D& triangle)
{
	Plane3D plane;
	plane.normal = (triangle.b - triangle.a).Cross(triangle.c - triangle.a);
	plane.normal.Normalize();

	plane.offset = plane.normal.Dot(triangle.a);
	return plane;
}