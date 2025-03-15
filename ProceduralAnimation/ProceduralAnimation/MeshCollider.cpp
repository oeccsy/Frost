#include "pch.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"
#include "Mesh.h"
#include "MeshCollider.h"
#include <algorithm>

MeshCollider::MeshCollider() : Collider(ColliderType::Mesh) {}
MeshCollider::~MeshCollider() {}

void MeshCollider::Init()
{
	_mesh = GetOwner()->GetMesh();
}

bool MeshCollider::Intersects(Ray& ray, OUT float& distance)
{
	if (_mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) return false;

	bool hit = false;
	vector<Vertex>& vertices = _mesh.lock()->GetVertices();
	vector<uint32>& indices = _mesh.lock()->GetIndices();

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		float tempDist;
		
		if (Raycast(triangle, ray, tempDist))
		{
			distance = min(distance, tempDist);
			hit = true;
		}
	}

	return hit;
}

bool MeshCollider::Intersects(shared_ptr<Collider>& other)
{
	// TODO
	return false;
}

bool MeshCollider::Intersects(Circle3D& circle, OUT float& theta)
{
	if (!_mesh.lock()) _mesh = GetOwner()->GetMesh(); // TODO Init()
	if (_mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex>& vertices = _mesh.lock()->GetVertices();
	vector<uint32>& indices = _mesh.lock()->GetIndices();

	vector<float> thetaContainer;

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		Circlecast(triangle, circle, thetaContainer);
	}

	theta = *min_element(thetaContainer.begin(), thetaContainer.end());

	return !thetaContainer.empty();
}

Vector3 MeshCollider::Snap(Vector3 position)
{
	if (!_mesh.lock()) _mesh = GetOwner()->GetMesh(); // TODO Init()
	if (_mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex>& vertices = _mesh.lock()->GetVertices();
	vector<uint32>& indices = _mesh.lock()->GetIndices();

	Vector3 snapPos;
	float snapDistSquared = FLT_MAX;

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		Plane3D plane = Plane3D::FromTriangle(triangle);
		Ray ray({ position, plane.normal });

		float p_h_offset = plane.offset - plane.normal.Dot(position);
		if (p_h_offset < 0) ray.direction = -plane.normal;
		
		float pointToPlaneDist = 0.f;

		if (Raycast(plane, ray, pointToPlaneDist))
		{
			Vector3 planeHitPoint = ray.position + pointToPlaneDist * ray.direction;

			if (IsPointInTriangle(planeHitPoint, triangle))
			{
				float curDistSquared = Vector3::DistanceSquared(planeHitPoint, position);
				if (curDistSquared < snapDistSquared)
				{
					snapPos = planeHitPoint;
					snapDistSquared = curDistSquared;
				}
			}
			else
			{
				Line3D closestLine({ triangle.a, triangle.b });
				
				if (Vector3::DistanceSquared(triangle.c, position) < Vector3::DistanceSquared(triangle.a, position))
				{
					closestLine.start = triangle.c;
				}
				else if (Vector3::DistanceSquared(triangle.c, position) < Vector3::DistanceSquared(triangle.b, position))
				{
					closestLine.end = triangle.c;
				}

				Vector3 ab = closestLine.end - closestLine.start;
				Vector3 ap = position - closestLine.start;
				float ab_dot_ab = ab.Dot(ab);
				float ab_dot_ap = ab.Dot(ap);
				
				Vector3 closestPos;

				if (ab_dot_ap < 0)
				{
					closestPos = closestLine.start;
				}
				else if (ab_dot_ap > ab_dot_ab)
				{
					closestPos = closestLine.end;
				}
				else
				{
					float t = ab_dot_ap / ab_dot_ab;
					closestPos = closestLine.start + t * ab;
				}

				float curDistSquared = Vector3::DistanceSquared(closestPos, position);
				if (curDistSquared < snapDistSquared)
				{
					snapPos = closestPos;
					snapDistSquared = curDistSquared;
				}
			}
		}
	}

	return snapPos;
}

bool MeshCollider::Raycast(const Plane3D& plane, const Ray& ray, OUT float& distance)
{
	float n_dot_o = plane.normal.Dot(ray.position);
	float n_dot_d = plane.normal.Dot(ray.direction);
	if (n_dot_d < FLT_EPSILON) return false;
	
	float t = - (n_dot_o - plane.offset) / n_dot_d;
	if (t < 0) return false;

	distance = t;

	return true;
}

bool MeshCollider::Raycast(const Triangle3D& triangle, const Ray& ray, OUT float& distance)
{
	Plane3D plane = Plane3D::FromTriangle(triangle);
	if (!Raycast(plane, ray, OUT distance)) return false;

	return IsPointInTriangle(ray.position + distance * ray.direction, triangle);
}

bool MeshCollider::Circlecast(const Triangle3D& triangle, const Circle3D& circle, OUT vector<float>& theta)
{
	Plane3D plane = Plane3D::FromTriangle(triangle);

	// Plane.Normal * P(theta) = Plane.Offset -> A * cos(theta) + B * sin(theta) = C
	float A = circle.radius * plane.normal.Dot(circle.xAxis);
	float B = circle.radius * plane.normal.Dot(circle.yAxis);
	float C = plane.offset - (plane.normal.Dot(circle.center));

	float R = sqrt(A * A + B * B);
	float alpha = atan2(B, A);

	// A * cos(theta) + B * sin(theta) = R * cos(theta - alpha) = C
	if (R < fabs(C)) return false;

	float theta1 = alpha + acos(C / R);
	float theta2 = alpha - acos(C / R);

	// [0, 360') Normalize
	theta1 = fmod(theta1 + XM_2PI, XM_2PI);
	theta2 = fmod(theta2 + XM_2PI, XM_2PI);

	Vector3 point1 = circle.center + circle.radius * (cos(theta1) * circle.xAxis + sin(theta1) * circle.yAxis);
	Vector3 point2 = circle.center + circle.radius * (cos(theta2) * circle.xAxis + sin(theta2) * circle.yAxis);

	int prevThetaCount = theta.size();
	if (IsPointInTriangle(point1, triangle)) theta.push_back(theta1);
	if (IsPointInTriangle(point2, triangle)) theta.push_back(theta2);

	return prevThetaCount < theta.size();
}

bool MeshCollider::IsPointInPlane(const Point3D& point, const Plane3D& plane)
{
	float equation = plane.normal.Dot(point) - plane.offset;
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