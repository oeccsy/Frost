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
	
private:
	static int MAX_POINTS_PER_SECTION;
	vector<Vector3> _points;

	BoundingBox _bounds;
	unique_ptr<PointOctree> _children[8] = { nullptr };
};