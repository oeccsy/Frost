#include "Triangle3D.h"
#include "Utils/Math/Plane3D.h"
#include "Utils/Math/Circle3D.h"

bool Triangle3D::IsPointInside(const Vector3& point) const
{
	Vector3 ab = b - a;
	Vector3 bc = c - b;
	Vector3 ca = a - c;

	Vector3 ap = point - a;
	Vector3 bp = point - b;
	Vector3 cp = point - c;

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

bool Triangle3D::Raycast(const Ray& ray, OUT float& distance) const
{
	Plane3D plane = Plane3D::FromTriangle(*this);
	if (!plane.Raycast(ray, OUT distance)) return false;

	return IsPointInside(ray.position + distance * ray.direction);
}

bool Triangle3D::Circlecast(const Circle3D& circle, OUT std::vector<float>& theta) const
{
	Plane3D plane = Plane3D::FromTriangle(*this);

	// Plane.Normal * P(theta) = Plane.Offset -> A * cos(theta) + B * sin(theta) = C
	float A = circle.radius * plane.normal.Dot(circle.xAxis);
	float B = circle.radius * plane.normal.Dot(circle.yAxis);
	float C = plane.offset - (plane.normal.Dot(circle.center));

	float R = static_cast<float>(sqrt(A * A + B * B));
	float alpha = static_cast<float>(atan2(B, A));

	// A * cos(theta) + B * sin(theta) = R * cos(theta - alpha) = C
	if (R < fabs(C)) return false;

	float theta1 = alpha + acos(C / R);
	float theta2 = alpha - acos(C / R);

	// [0, 360') Normalize
	theta1 = static_cast<float>(fmod(theta1 + XM_2PI, XM_2PI));
	theta2 = static_cast<float>(fmod(theta2 + XM_2PI, XM_2PI));

	Vector3 point1 = circle.center + circle.radius * ((float)cos(theta1) * circle.xAxis + (float)sin(theta1) * circle.yAxis);
	Vector3 point2 = circle.center + circle.radius * ((float)cos(theta2) * circle.xAxis + (float)sin(theta2) * circle.yAxis);

	int prev_theta_count = theta.size();
	if (IsPointInside(point1)) theta.push_back(theta1);
	if (IsPointInside(point2)) theta.push_back(theta2);

	return prev_theta_count < theta.size();
}
