#pragma once

class PointOctree
{
public:
	PointOctree(const BoundingBox bounds);
	virtual ~PointOctree();

public:
	bool IsLeaf();
	void Insert(Vector3 point);
	void Subdivide();
	
public:
	bool IsInclude(Vector3 position);
	bool IntersectsWithBounds(const BoundingSphere& boundingSphere);
	bool IntersectsWithPoints(const BoundingSphere& boundingSphere);
	
private:
	static const int MAX_POINTS_PER_SECTION = 10;
	vector<Vector3> _points;

	BoundingBox _bounds;
	shared_ptr<PointOctree> _children[8] = { nullptr };
};