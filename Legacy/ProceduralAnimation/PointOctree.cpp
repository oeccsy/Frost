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
    if (!IsInclude(point)) return;
    
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

bool PointOctree::IsInclude(Vector3 position)
{
    Vector3 center = _bounds.Center;
    Vector3 size = _bounds.Extents;

    Vector3 maxBounds = center + size;
    Vector3 minBounds = center - size;

    if (position.x < minBounds.x || maxBounds.x < position.x) return false;
    if (position.y < minBounds.y || maxBounds.y < position.y) return false;
    if (position.z < minBounds.z || maxBounds.z < position.z) return false;

    return true;
}

bool PointOctree::IntersectsWithBounds(const BoundingSphere& boundingSphere)
{
    return _bounds.Intersects(boundingSphere);
}

bool PointOctree::IntersectsWithPoints(const BoundingSphere& boundingSphere)
{
    if (!IntersectsWithBounds(boundingSphere)) return false;

    if (!IsLeaf())
    {
        for (int i = 0; i < 8; i++)
        {
            if (_children[i]->IntersectsWithPoints(boundingSphere)) return true;
        }

        return false;
    }
    else
    {
        float rSquared = boundingSphere.Radius * boundingSphere.Radius;

        for (Vector3 point : _points)
        {
            if (Vector3::DistanceSquared(point, boundingSphere.Center) <= rSquared) return true;
        }

        return false;
    }
}