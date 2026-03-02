#include "MeshCollider.h"
#include "Object/Object.h"
#include "Data/Vertex.h"
#include "Render/Mesh.h"
#include "Utils/Math/Point3D.h"
#include "Utils/Math/Line3D.h"
#include "Utils/Math/Plane3D.h"
#include "Utils/Math/Triangle3D.h"
#include "Utils/Math/Circle3D.h"
#include "SpatialPartitioning/TriangleOctree.h"
#include <algorithm>
#include <stack>

MeshCollider::MeshCollider() : Collider(ColliderType::Mesh) {}

MeshCollider::~MeshCollider() {}

void MeshCollider::Awake()
{
	mesh = GetOwner()->GetMesh();
	triangle_octree = make_shared<TriangleOctree>(BoundingBox({ Vector3(0, 0, 0), Vector3(6, 6, 6) }));

	const vector<Vertex>& vertices = mesh.lock()->GetVertices();
	const vector<uint32>& indices = mesh.lock()->GetIndices();

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		triangle_octree->Insert(triangle);
	}
}

bool MeshCollider::Intersects(Ray& ray, OUT float& distance)
{
	if (mesh.lock() == nullptr) return false;
	if (mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) return false;

	const vector<Vertex>& vertices = mesh.lock()->GetVertices();
	const vector<uint32>& indices = mesh.lock()->GetIndices();

	bool hit = false;

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		float temp_dist;

		if (triangle.Raycast(ray, temp_dist))
		{
			distance = min(distance, temp_dist);
			hit = true;
		}
	}

	return hit;
}

bool MeshCollider::Intersects(shared_ptr<Collider>& other)
{
	return false;
}

bool MeshCollider::Intersects(Circle3D& circle, OUT float& theta)
{
	if (mesh.lock() == nullptr) return false;
	if (mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) false;

	const vector<Vertex>& vertices = mesh.lock()->GetVertices();
	const vector<uint32>& indices = mesh.lock()->GetIndices();

	vector<float> theta_container;

	stack<shared_ptr<TriangleOctree>> dfs_stack;
	dfs_stack.push(triangle_octree);

	BoundingSphere check_bounds({ circle.center, circle.radius });

	while (!dfs_stack.empty())
	{
		shared_ptr<TriangleOctree> cur_octree = dfs_stack.top();
		dfs_stack.pop();

		for (auto& triangle : cur_octree->GetTriangles())
		{
			triangle.Circlecast(circle, theta_container);
		}

		if (!cur_octree->IsLeaf())
		{
			for (int i = 0; i < 8; i++)
			{
				shared_ptr<TriangleOctree> child = cur_octree->GetChild(i);
				if (child->IntersectsWithBounds(check_bounds)) dfs_stack.push(child);
			}
		}
	}

	if (!theta_container.empty())
	{
		theta = *min_element(theta_container.begin(), theta_container.end());
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 MeshCollider::Snap(Vector3 position)
{
	if (mesh.lock()->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	const vector<Vertex>& vertices = mesh.lock()->GetVertices();
	const vector<uint32>& indices = mesh.lock()->GetIndices();

	Vector3 snap_pos;
	float snap_dist_squared = FLT_MAX;

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle3D triangle{ vertices[indices[i]].position, vertices[indices[i + 1]].position, vertices[indices[i + 2]].position };
		Plane3D plane = Plane3D::FromTriangle(triangle);
		Ray ray({ position, plane.normal });

		float p_h_offset = plane.offset - plane.normal.Dot(position);
		if (p_h_offset < 0) ray.direction = -plane.normal;

		float point_to_plane_dist = 0.f;

		if (plane.Raycast(ray, point_to_plane_dist))
		{
			Vector3 plane_hit_point = ray.position + point_to_plane_dist * ray.direction;

			if (triangle.IsPointInside(plane_hit_point))
			{
				float cur_dist_squared = Vector3::DistanceSquared(plane_hit_point, position);
				if (cur_dist_squared < snap_dist_squared)
				{
					snap_pos = plane_hit_point;
					snap_dist_squared = cur_dist_squared;
				}
			}
			else
			{
				Line3D closest_line({ triangle.a, triangle.b });

				if (Vector3::DistanceSquared(triangle.c, position) < Vector3::DistanceSquared(triangle.a, position))
				{
					closest_line.start = triangle.c;
				}
				else if (Vector3::DistanceSquared(triangle.c, position) < Vector3::DistanceSquared(triangle.b, position))
				{
					closest_line.end = triangle.c;
				}

				Vector3 ab = closest_line.end - closest_line.start;
				Vector3 ap = position - closest_line.start;
				float ab_dot_ab = ab.Dot(ab);
				float ab_dot_ap = ab.Dot(ap);

				Vector3 closest_pos;

				if (ab_dot_ap < 0)
				{
					closest_pos = closest_line.start;
				}
				else if (ab_dot_ap > ab_dot_ab)
				{
					closest_pos = closest_line.end;
				}
				else
				{
					float t = ab_dot_ap / ab_dot_ab;
					closest_pos = closest_line.start + t * ab;
				}

				float cur_dist_squared = Vector3::DistanceSquared(closest_pos, position);
				if (cur_dist_squared < snap_dist_squared)
				{
					snap_pos = closest_pos;
					snap_dist_squared = cur_dist_squared;
				}
			}
		}
	}

	return snap_pos;
}