#pragma once

#include "Core.h"
#include "Utils/Math/Triangle3D.h"
#include "Types.h"
#include <vector>
#include <memory>

class TriangleOctree
{
public:
	TriangleOctree(const BoundingBox bounds);
	virtual ~TriangleOctree();

	FORCEINLINE bool IsLeaf() const { return children[0] == nullptr; }
	bool Insert(Triangle3D triangle);
	void Subdivide();

	bool IsInclude(Triangle3D triangle);
	bool IntersectsWithBounds(const BoundingSphere& boundingSphere);

	FORCEINLINE shared_ptr<TriangleOctree> GetChild(int index) { return children[index]; }
	FORCEINLINE vector<Triangle3D>& GetTriangles() { return triangles; }

private:
	BoundingBox bounds;
	shared_ptr<TriangleOctree> children[8] = { nullptr };

	static const int MAX_TRIANGLES_PER_SECTION = 10;
	vector<Triangle3D> triangles;
};