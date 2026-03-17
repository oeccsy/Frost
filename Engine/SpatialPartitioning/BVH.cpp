#include "BVH.h"
#include "Types.h"

BVH::BVH() {}

BVH::~BVH() {}

void BVH::Build(vector<Triangle3D>& target_triangles)
{
    triangles = target_triangles;

    if (triangles.empty())
    {
        root = nullptr;
        return;
    }

    root = BuildRecursive(0, static_cast<int>(triangles.size()));
}

shared_ptr<BVHNode> BVH::BuildRecursive(int start, int end)
{
    int count = end - start;
    if (count == 0) return nullptr;

    shared_ptr<BVHNode> new_node = make_shared<BVHNode>();
    new_node->bounds = ComputeBounds(start, end);
    new_node->start_index = start;
    new_node->end_index = end;

    if (count <= MAX_LEAF_TRIANGLES) return new_node;

    BVHSplitInfo split_info = FindBestSplitSAH(start, end, new_node->bounds);

    int left = start;
    int right = end - 1;

    while (left <= right)
    {
        Vector3 center = triangles[left].CalculateCenterPosition();

        if (IsBelowSplitThreshold(center, split_info))
        {
            ++left;
        }
        else
        {
            swap(triangles[left], triangles[right]);
            --right;
        }
    }

    int mid_index = left;
    if (mid_index == start || mid_index == end) return new_node;
    
    new_node->left_child = BuildRecursive(start, mid_index);
    new_node->right_child = BuildRecursive(mid_index, end);

    return new_node;
}

BoundingBox BVH::ComputeBounds(int start, int end) const
{
    BoundingBox result;
    BoundingBox::CreateFromPoints(result, 3, triangles[start].points, sizeof(Vector3));

    for (int i = start + 1; i < end; ++i)
    {
        BoundingBox triangle_bounds;
        BoundingBox::CreateFromPoints(triangle_bounds, 3, triangles[i].points, sizeof(Vector3));
        BoundingBox::CreateMerged(result, result, triangle_bounds);
    }

    return result;
}


BVHSplitInfo BVH::FindBestSplitSAH(int start, int end, const BoundingBox& bounds)
{
    BVHSplitInfo best_split_info;

    if (bounds.Extents.x > bounds.Extents.y && bounds.Extents.x > bounds.Extents.z)
    {
        best_split_info.axis = 0;
        best_split_info.split_value = bounds.Center.x;
    }
    else if (bounds.Extents.y > bounds.Extents.z)
    {
        best_split_info.axis = 1;
        best_split_info.split_value = bounds.Center.y;
    }
    else
    {
        best_split_info.axis = 2;
        best_split_info.split_value = bounds.Center.z;
    }

    return best_split_info;
}

bool BVH::IsBelowSplitThreshold(Vector3 position, BVHSplitInfo info)
{
    switch (info.axis)
    {
    case 0:
        return position.x < info.split_value;
    case 1:
        return position.y < info.split_value;
    case 2:
        return position.z < info.split_value;
    default:
        return false;
    }
}

float BVH::EvaluateBoundsCost(const BoundingBox& bounds, int triangle_count) const
{
    Vector3 size = static_cast<Vector3>(bounds.Extents) * 2;
    float half_area = size.x * size.y + size.x * size.z + size.y * size.z;
    float cost = half_area * triangle_count;

    return (cost > 0.0f) ? cost : FLT_MAX;
}