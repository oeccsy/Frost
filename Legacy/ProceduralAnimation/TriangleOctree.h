#pragma once

class TriangleOctree
{
public:
	TriangleOctree(const BoundingBox bounds);
	virtual ~TriangleOctree();

public:
	bool IsLeaf();
	bool Insert(Triangle3D triangle);
	void Subdivide();

public:
	bool IsInclude(Triangle3D triangle);
	bool IntersectsWithBounds(const BoundingSphere& boundingSphere);
	vector<Triangle3D>& GetTriangles() { return _triangles; }
	shared_ptr<TriangleOctree> GetChild(int index) { return _children[index]; }

private:
	static const int MAX_TRIANGLES_PER_SECTION = 10;
	vector<Triangle3D> _triangles;

	BoundingBox _bounds;
	shared_ptr<TriangleOctree> _children[8] = { nullptr };
};