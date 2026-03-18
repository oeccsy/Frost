#pragma once

#include "Core.h"
#include "Utils/Math/Triangle3D.h"
#include "Types.h"
#include "Data/BVHNode.h"
#include "Data/BVHSplitInfo.h"
#include <vector>
#include <memory>

class ENGINE_API BVH
{
public:
	BVH();
	virtual ~BVH();

	void Build(const vector<Triangle3D>& target_triangles);
	FORCEINLINE shared_ptr<BVHNode> GetRoot() const { return root; }
	FORCEINLINE const vector<Triangle3D>& GetTriangles() const { return triangles; }

private:
	shared_ptr<BVHNode> BuildRecursive(int start, int end);
	BoundingBox ComputeBounds(int start, int end) const;

	BVHSplitInfo FindLongestAxisSplit(const BoundingBox& bounds) const;
	BVHSplitInfo FindBestSAHSplit(int start, int end, const BoundingBox& bounds) const;
	float EvaluateSplit(int axis, float split_value, int start, int end) const;
	float EvaluateBoundsCost(const BoundingBox& bounds, int triangle_count) const;

private:
	shared_ptr<BVHNode> root;
	vector<Triangle3D> triangles;

	static const int MAX_LEAF_TRIANGLES = 4;
};