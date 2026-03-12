#pragma once

#include "Core.h"
#include "Types.h"
#include <vector>

class FrostRoot
{
public:
    FrostRoot(Vector3 base_point, Vector3 normal);
    virtual ~FrostRoot();
    
    void Fork(shared_ptr<class MeshCollider> guide_mesh_collider);

    void GrowBranches(shared_ptr<class MeshCollider> guide_mesh_collider);
    void ForkBranches(shared_ptr<class MeshCollider> guide_mesh_collider);
    void StopCloseBranches(shared_ptr<class PointCloud> frost_points);
    void UpdateGrowingBranches();
    
    FORCEINLINE Vector3 GetBasePoint() const { return base_point; }
    FORCEINLINE const vector<shared_ptr<class FrostBranch>>& GetGrowingBranches() const { return growing_branches; }
    FORCEINLINE const vector<shared_ptr<class FrostBranch>>& GetNewBranches() const { return new_branches; }
    
private:
    Vector3 base_point;
    Vector3 normal;
    
    vector<shared_ptr<class FrostBranch>> growing_branches;
    vector<shared_ptr<class FrostBranch>> new_branches;
};