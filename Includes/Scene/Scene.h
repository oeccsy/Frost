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
    
    virtual void Awake();
    virtual void Start();
    virtual void Update(float delta_time);
    virtual void LateUpdate();
    virtual void Render();
    
    void ProcessAddAndDestroyObjects();

	template<typename T>
    shared_ptr<T> SpawnObject()
    {
		shared_ptr<T> new_object = make_shared<T>();
		new_object->Awake();
		add_requested_objects.push_back(new_object);
		return new_object;
    }
    
protected:
    vector<shared_ptr<class Object>> objects;
    
    vector<shared_ptr<class Object>> add_requested_objects;
    vector<shared_ptr<class Object>> destroy_requested_objects;
};