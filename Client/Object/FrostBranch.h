#pragma once

#include "Core.h"
#include "Types.h"
#include "Utils/Math/Circle3D.h"
#include <vector>

class FrostBranch : public enable_shared_from_this<FrostBranch>
{
public:
    FrostBranch(Vector3& base_point, Vector3& dir, Vector3& normal, shared_ptr<FrostBranch> parent);
    virtual ~FrostBranch();

    void Grow(shared_ptr<class MeshCollider> guide_mesh_collider);
    bool Fork(shared_ptr<class MeshCollider> guide_mesh_collider);

public:
    FORCEINLINE Vector3 GetEndPoint() const { return points.back(); }
    FORCEINLINE Vector3 GetPrevEndPoint() const { return points[points.size() - 2]; }
    FORCEINLINE bool HasParent() const { return parent != nullptr; }
    FORCEINLINE shared_ptr<FrostBranch> GetParent() const { return parent; }
    FORCEINLINE const vector<shared_ptr<FrostBranch>>& GetChildren() const { return children; }

private:
    vector<Vector3> points;
    
    shared_ptr<FrostBranch> parent;
    vector<shared_ptr<FrostBranch>> children;
    
    Circle3D guide_circle;
};