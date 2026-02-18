#pragma once

#include "Core.h"
#include "RTTI.h"
#include <vector>
#include <memory>

class ENGINE_API Scene : public RTTI
{
    RTTI_DECLARATIONS(Scene, RTTI)
    
public:
    Scene();
    virtual ~Scene();
    
    void AddObject(shared_ptr<class Object> new_object);
    void DestroyObject(shared_ptr<class Object> destroyed_object);
    
    virtual void Init();
    virtual void Update(float delta_time);
    virtual void LateUpdate();
    virtual void Render();
    
    void ProcessAddAndDestroyObjects();
    
protected:
    vector<shared_ptr<class Object>> objects;
    
    vector<shared_ptr<class Object>> add_requested_objects;
    vector<shared_ptr<class Object>> destroy_requested_objects;
};