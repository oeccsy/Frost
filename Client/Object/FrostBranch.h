#pragma once

#include "Object/Object.h"
#include "Types.h"
#include "Utils/Math/Circle3D.h"
#include "Data/Vertex.h"

class FrostBranch : public Object
{
public:
    FrostBranch(Vector3& base_point, Vector3& dir, Vector3& normal, shared_ptr<FrostBranch> parent);
    virtual ~FrostBranch() override;

    virtual void Awake() override;
    bool Grow(shared_ptr<class MeshCollider> target);
    bool Fork(shared_ptr<class MeshCollider> target);

public:
    shared_ptr<FrostBranch>& GetParent() { return parent; }
    vector<shared_ptr<FrostBranch>>& GetChildren() { return children; }
    Vector3 GetBranchEndPos() { return branch.back().position; };

private:
    Circle3D guide_circle;

    vector<Vertex> branch;

    shared_ptr<FrostBranch> parent;
    vector<shared_ptr<FrostBranch>> children;
};