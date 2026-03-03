#pragma once

#include "Core.h"
#include "Types.h"
#include <unordered_set>

class FrostRoot
{
public:
    FrostRoot(Vector3 base_point, Vector3 normal);
    virtual ~FrostRoot();

    void Fork(shared_ptr<class MeshCollider> target);
    void Grow(shared_ptr<class MeshCollider> target);

    void StopIntersectingBranches(shared_ptr<class PointCloud> frost_points);
    vector<Vector3> GetLatestEndPoints();

    Vector3& GetBasePoint() { return base_point; }

private:
    Vector3 base_point;
    Vector3 normal;

    vector<shared_ptr<class FrostBranch>> branches;
    unordered_set<shared_ptr<class FrostBranch>> growing_branches;
    unordered_set<shared_ptr<class FrostBranch>> growing_main_branches;
};