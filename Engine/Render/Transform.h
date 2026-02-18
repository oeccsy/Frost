#pragma once

#include "Core.h"
#include "Types.h"
#include <memory>
#include <vector>

class ENGINE_API Transform
{
public:
    Transform();
    virtual ~Transform();
    
    FORCEINLINE Vector3 GetLocalPosition() const { return local_position; }
    FORCEINLINE Vector3 GetWorldPosition() const { return world_position; }
    FORCEINLINE Quaternion GetLocalRotation() const { return local_rotation; }
    FORCEINLINE Quaternion GetWorldRotation() const { return world_rotation; }
    FORCEINLINE Vector3 GetLocalScale() const { return local_scale; }
    FORCEINLINE Vector3 GetWorldScale() const { return world_scale; }

    void SetLocalPosition(const Vector3& position);
    void SetWorldPosition(const Vector3& position);
    void SetLocalRotation(const Quaternion& rotation);
    void SetWorldRotation(const Quaternion& rotation);
    void SetLocalScale(const Vector3& scale);
    void SetWorldScale(const Vector3& scale);
    
    FORCEINLINE Vector3 GetForward() const { return Vector3::TransformNormal(Vector3::Forward, world_matrix); }
    FORCEINLINE Vector3 GetRight() const { return Vector3::TransformNormal(Vector3::Right, world_matrix); }
    FORCEINLINE Vector3 GetUp() const { return Vector3::TransformNormal(Vector3::Up, world_matrix); }
    FORCEINLINE Matrix GetWorldMatrix() const { return world_matrix; }
    
    FORCEINLINE shared_ptr<Transform> GetParent() const { return parent.lock(); }
    FORCEINLINE const vector<shared_ptr<Transform>>& GetChildren() { return children; }

    FORCEINLINE void SetParent(const shared_ptr<Transform>& new_parent) { parent = new_parent; }
    FORCEINLINE void AddChild(const shared_ptr<Transform>& new_child) { children.push_back(new_child); }

private:
    void RefreshMatrix();
    void RefreshWorldTRS();

private:
    Vector3 local_position = Vector3::Zero;
    Vector3 world_position = Vector3::Zero;
    Quaternion local_rotation = Quaternion::Identity;
    Quaternion world_rotation = Quaternion::Identity;
    Vector3 local_scale = Vector3::One;
    Vector3 world_scale = Vector3::One;

    Matrix local_matrix = Matrix::Identity;
    Matrix world_matrix = Matrix::Identity;

    weak_ptr<Transform> parent;
    vector<shared_ptr<Transform>> children;
};