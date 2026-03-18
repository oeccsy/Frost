#pragma once

#include "Core.h"
#include <memory>

struct ENGINE_API BVHNode
{
	BoundingBox bounds;

	int start_index;
	int end_index;
	
	shared_ptr<BVHNode> left_child;
	shared_ptr<BVHNode> right_child;

	FORCEINLINE bool IsLeaf() const { return left_child == nullptr && right_child == nullptr; }
	FORCEINLINE bool IntersectsWithBounds(const BoundingSphere& boundingSphere) { return bounds.Intersects(boundingSphere); }
};