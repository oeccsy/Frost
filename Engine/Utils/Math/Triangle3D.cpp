#include "Triangle3D.h"

bool Triangle3D::IsPointInside(const Point3D& point) const
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
