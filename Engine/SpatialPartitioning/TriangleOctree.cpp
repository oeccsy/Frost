#include "TriangleOctree.h"

TriangleOctree::TriangleOctree(const BoundingBox bounds) : bounds(bounds) { }

TriangleOctree::~TriangleOctree() {}

bool TriangleOctree::Insert(Triangle3D triangle)
{
	if (!IsInclude(triangle)) return false;

	if (IsLeaf())
	{
		triangles.push_back(triangle);
		if (triangles.size() > MAX_TRIANGLES_PER_SECTION) Subdivide();

		return true;
	}
	else
	{
		bool insert_success = false;

		for (int i = 0; i < 8; i++)
		{
			if (children[i]->Insert(triangle))
			{
				insert_success = true;
				break;
			}
		}

		if (!insert_success) triangles.push_back(triangle);

		return true;
	}
}

void TriangleOctree::Subdivide()
{
	if (!IsLeaf()) return;

	Vector3 cur_bounds = bounds.Extents;
	Vector3 cur_center = bounds.Center;

	Vector3 next_bounds = cur_bounds * 0.5f;

	for (int i = 0; i < 8; i++)
	{
		Vector3 next_center;

		next_center.x = cur_center.x + ((i & 1) ? next_bounds.x : -next_bounds.x);
		next_center.y = cur_center.y + ((i & 2) ? next_bounds.y : -next_bounds.y);
		next_center.z = cur_center.z + ((i & 4) ? next_bounds.z : -next_bounds.z);

		children[i] = make_shared<TriangleOctree>(BoundingBox{ next_center, next_bounds });
	}


	vector<Triangle3D> stay;

	for (Triangle3D& triangle : triangles)
	{
		bool insert_success = false;

		for (int i = 0; i < 8; i++)
		{
			if (children[i]->Insert(triangle))
			{
				insert_success = true;
				break;
			}
		}

		if (!insert_success) stay.push_back(triangle);
	}

	triangles = stay;
}

bool TriangleOctree::IsInclude(Triangle3D triangle)
{
	Vector3 center = bounds.Center;
	Vector3 size = bounds.Extents;

	Vector3 max_bounds = center + size;
	Vector3 min_bounds = center - size;

	for (int i = 0; i < 3; i++)
	{
		Vector3 position = triangle.points[i];

		if (position.x < min_bounds.x || max_bounds.x < position.x) return false;
		if (position.y < min_bounds.y || max_bounds.y < position.y) return false;
		if (position.z < min_bounds.z || max_bounds.z < position.z) return false;
	}

	return true;
}

bool TriangleOctree::IntersectsWithBounds(const BoundingSphere& boundingSphere)
{
	return bounds.Intersects(boundingSphere);
}
