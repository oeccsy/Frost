#include "pch.h"
#include "Component.h"
#include "Collider.h"
#include "MeshCollider.h"

MeshCollider::~MeshCollider() {}

bool MeshCollider::Intersects(Ray& ray, OUT float& distance)
{
	return false;
}

bool MeshCollider::Intersects(shared_ptr<Collider>& other)
{
	return false;
}

bool MeshCollider::IsPointInPlane(const Point3D& point, const Plane3D& plane)
{
	float equation = plane.normal.Dot(point) - plane.distance;
	return fabs(equation) < FLT_EPSILON;
}

bool MeshCollider::IsPointInTriangle(const Point3D& point, const Triangle3D& triangle)
{
	Vector3 ab = triangle.b - triangle.a;
	Vector3 bc = triangle.c - triangle.b;
	Vector3 ca = triangle.a - triangle.c;

	Vector3 ap = point - triangle.a;
	Vector3 bp = point - triangle.b;
	Vector3 cp = point - triangle.c;

	Vector3 cross_a = ab.Cross(ap);
	Vector3 cross_b = bc.Cross(bp);
	Vector3 cross_c = ca.Cross(cp);

	float dot_a = cross_a.Dot(cross_b);
	float dot_b = cross_b.Dot(cross_c);
	float dot_c = cross_c.Dot(cross_a);

	if ((dot_a > 0 && dot_b > 0 && dot_c > 0) || (dot_a < 0 && dot_b < 0 && dot_c < 0))
		return true;
	else
		return false;
}

bool MeshCollider::Raycast(const Plane3D& plane, const Ray3D& ray, OUT Point3D& hitPoint)
{
	float n_dot_o = plane.normal.Dot(ray.origin);
	float n_dot_d = plane.normal.Dot(ray.direction);

	if (n_dot_d < FLT_EPSILON) return false;
	
	float t = - (n_dot_o - plane.distance) / n_dot_d;
	
	if (t < 0) return false;

	hitPoint = ray.origin + t * ray.direction;
	return true;
}

bool MeshCollider::Raycast(const Triangle3D& triangle, const Ray3D& ray, OUT Point3D& hitPoint)
{
	Plane3D plane = Plane3D::FromTriangle(triangle);
	if (!Raycast(plane, ray, OUT hitPoint)) return false;

	return IsPointInTriangle(hitPoint, triangle);
}