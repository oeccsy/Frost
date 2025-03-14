#include "pch.h"
#include "PointOctree.h"

PointOctree::PointOctree(const BoundingBox bounds)
{
	_bounds = bounds;
}

PointOctree::~PointOctree() {}

bool PointOctree::IsLeaf()
{
	return _children[0] == nullptr;
}

void PointOctree::Insert(Vector3 point)
{
    Vector3 center = _bounds.Center;
    Vector3 size = _bounds.Extents;

    Vector3 maxBounds = center + size;
    Vector3 minBounds = center - size;

    if (point.x < minBounds.x || maxBounds.x < point.x) return;
    if (point.y < minBounds.y || maxBounds.y < point.y) return;
    if (point.z < minBounds.z || maxBounds.z < point.z) return;
    
    if (IsLeaf())
    {
        _points.push_back(point);
        if (_points.size() > MAX_POINTS_PER_SECTION) Subdivide();
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            _children[i]->Insert(point);
        }
    }
}

void PointOctree::Subdivide()
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

        _children[i] = make_unique<PointOctree>(BoundingBox{ nextCenter, nextBounds });
    }

    for (Vector3 point : _points)
    {
        for (int i = 0; i < 8; i++)
        {
            _children[i]->Insert(point);
        }
    }

    _points.clear();
}
