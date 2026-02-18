#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Component/Component.h"
#include <vector>
#include <memory>

class ENGINE_API Object : public RTTI, public enable_shared_from_this<Object>
{
    RTTI_DECLARATIONS(Object, RTTI)

public:
    Object();
    virtual ~Object();

    virtual void Awake();
    virtual void Update();
    virtual void Render();
    
    void AddComponent(shared_ptr<class Component> component);
    const vector<shared_ptr<class Component>>& GetComponents() const { return components; }

    template <typename T>
    shared_ptr<T> GetComponent()
    {
        for (const auto& component : components)
        {
            if (component->As<T>() != nullptr) return component;
        }

        return nullptr;
    }
    
    FORCEINLINE shared_ptr<class Mesh> GetMesh() { return mesh; }
    FORCEINLINE shared_ptr<class Material> GetMaterial() { return material; }
    FORCEINLINE shared_ptr<class Transform> GetTransform() { return transform; }

protected:
    vector<shared_ptr<Component>> components;
    
    shared_ptr<class Mesh> mesh;
    shared_ptr<class Material> material;
    shared_ptr<class Transform> transform;
};