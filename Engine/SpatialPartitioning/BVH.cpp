#include "BVH.h"
#include "Types.h"

BVH::BVH() {}

BVH::~BVH() {}

void BVH::Build(const vector<Triangle3D>& target_triangles)
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

    BVHSplitInfo split_info = FindLongestAxisSplit(new_node->bounds);

    int left = start;
    int right = end - 1;

    while (left <= right)
    {
        Vector3 center = triangles[left].CalculateCenterPosition();
        float center_elem[3] = { center.x, center.y, center.z };

        if (center_elem[split_info.axis] < split_info.split_value)
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

BVHSplitInfo BVH::FindLongestAxisSplit(const BoundingBox& bounds) const
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

BVHSplitInfo BVH::FindBestSAHSplit(int start, int end, const BoundingBox& bounds) const
{
    BVHSplitInfo best_split_info = { 0, 0, FLT_MAX };
    const int NUM_BUCKETS = 8;

    Vector3 center = bounds.Center;
    Vector3 extents = bounds.Extents;
    Vector3 max_point = center + extents;
    Vector3 min_point = center - extents;

    float max_point_elem[3] = { max_point.x, max_point.y, max_point.z };
    float min_point_elem[3] = { min_point.x, min_point.y, min_point.z };
    
    for (int axis = 0; axis < 3; ++axis)
    {
        float max_value = max_point_elem[axis];
        float min_value = min_point_elem[axis];

        for (int i = 1; i <= NUM_BUCKETS; ++i)
        {
            float split_value = min_value + (max_value - min_value) * (i / (float)(NUM_BUCKETS + 1));
            float cost = EvaluateSplit(axis, split_value, start, end);

            if (cost < best_split_info.cost)
            {
                best_split_info = { axis, split_value, cost };
            }
        }
    }

    return best_split_info;
}

float BVH::EvaluateSplit(int axis, float split_value, int start, int end) const
{
    BoundingBox bounds_a;
    BoundingBox bounds_b;
    int count_a = 0;
    int count_b = 0;

    for (int i = start; i < end; ++i)
    {
        BoundingBox triangle_bounds;
        BoundingBox::CreateFromPoints(triangle_bounds, 3, triangles[i].points, sizeof(Vector3));

        Vector3 center = triangles[i].CalculateCenterPosition();
        float center_elem[3] = { center.x, center.y, center.z };

        if (center_elem[axis] < split_value)
        {
            if (count_a == 0)
            { 
                bounds_a = triangle_bounds;
            }
            else
            {
                BoundingBox::CreateMerged(bounds_a, bounds_a, triangle_bounds);
            }

            ++count_a;
        }
        else
        {
            if (count_b == 0)
            {
                bounds_b = triangle_bounds;
            }
            else
            {
                BoundingBox::CreateMerged(bounds_b, bounds_b, triangle_bounds);
            }

            ++count_b;
        }
    }

    float cost_a = EvaluateBoundsCost(bounds_a, count_a);
    float cost_b = EvaluateBoundsCost(bounds_b, count_b);
    if (cost_a == FLT_MAX || cost_b == FLT_MAX) return FLT_MAX;

    return cost_a + cost_b;
}

float BVH::EvaluateBoundsCost(const BoundingBox& bounds, int triangle_count) const
{
    Vector3 size = static_cast<Vector3>(bounds.Extents) * 2;
    float half_area = size.x * size.y + size.x * size.z + size.y * size.z;
    float cost = half_area * triangle_count;

    return (cost > 0.0f) ? cost : FLT_MAX;
}