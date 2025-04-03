#include "pch.h"
#include "TriangleOctree.h"

TriangleOctree::TriangleOctree(const BoundingBox bounds)
{
	_bounds = bounds;
}

TriangleOctree::~TriangleOctree() {}

bool TriangleOctree::IsLeaf()
{
	return _children[0] == nullptr;
}

bool TriangleOctree::Insert(Triangle3D triangle)
{
	if (!IsInclude(triangle)) return false;

	if (IsLeaf())
	{
		_triangles.push_back(triangle);
		if (_triangles.size() > MAX_TRIANGLES_PER_SECTION) Subdivide();

		return true;
	}
	else
	{
		bool insertSuccess = false;

		for (int i = 0; i < 8; i++)
		{
			if (_children[i]->Insert(triangle))
			{
				insertSuccess = true;
				break;
			}
		}

		if (!insertSuccess) _triangles.push_back(triangle);

		return true;
	}
}

void TriangleOctree::Subdivide()
{
	if (!IsLeaf()) return;

	Vector3 curBounds = _bounds.Extents;
	Vector3 curCenter = _bounds.Center;

	Vector3 nextBounds = curBounds * 0.5f;

	for (int i = 0; i < 8; i++)
	{
		Vector3 nextCenter;

		nextCenter.x = curCenter.x + ((i & 1) ? nextBounds.x : -nextBounds.x);
		nextCenter.y = curCenter.y + ((i & 2) ? nextBounds.y : -nextBounds.y);
		nextCenter.z = curCenter.z + ((i & 4) ? nextBounds.z : -nextBounds.z);

		_children[i] = make_unique<TriangleOctree>(BoundingBox{ nextCenter, nextBounds });
	}


	vector<Triangle3D> stay;

	for (Triangle3D& triangle : _triangles)
	{
		bool insertSuccess = false;

		for (int i = 0; i < 8; i++)
		{
			if (_children[i]->Insert(triangle))
			{
				insertSuccess = true;
				break;
			}
		}

		if (!insertSuccess) stay.push_back(triangle);
	}

	_triangles = stay;
}

bool TriangleOctree::IsInclude(Triangle3D triangle)
{
	Vector3 center = _bounds.Center;
	Vector3 size = _bounds.Extents;

	Vector3 maxBounds = center + size;
	Vector3 minBounds = center - size;

	for (int i = 0; i < 3; i++)
	{
		Vector3 position = triangle.points[i];

		if (position.x < minBounds.x || maxBounds.x < position.x) return false;
		if (position.y < minBounds.y || maxBounds.y < position.y) return false;
		if (position.z < minBounds.z || maxBounds.z < position.z) return false;
	}

	return true;
}

bool TriangleOctree::IntersectsWithBounds(const BoundingSphere& boundingSphere)
{
	return _bounds.Intersects(boundingSphere);
}
