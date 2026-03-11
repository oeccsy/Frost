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
    void Grow(shared_ptr<class MeshCollider> guide_mesh_collider);
    bool Fork(shared_ptr<class MeshCollider> guide_mesh_collider);

public:
    FORCEINLINE Vector3 GetEndPoint() const { return points.back(); }
    FORCEINLINE bool HasParent() const { return parent != nullptr; }
    FORCEINLINE shared_ptr<FrostBranch> GetParent() const { return parent; }
    FORCEINLINE const vector<shared_ptr<FrostBranch>>& GetChildren() const { return children; }

protected:
    void CreateGeometry();
    void CreateShader();
    void CreateInputLayout();

private:
    vector<Vector3> points;
    
    shared_ptr<FrostBranch> parent;
    vector<shared_ptr<FrostBranch>> children;
    
    Circle3D guide_circle;
};