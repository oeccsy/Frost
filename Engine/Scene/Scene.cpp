#include "Scene.h"
#include "Object/Object.h"
#include "Render/Camera.h"

Scene::Scene() { }

Scene::~Scene() { }

void Scene::AddObject(shared_ptr<class Object> new_object)
{
    add_requested_objects.push_back(new_object);
}

void Scene::DestroyObject(shared_ptr<class Object> target_object)
{
    destroy_requested_objects.push_back(target_object);
}

void Scene::Awake() { }

void Scene::Start()
{
    for (auto& object : objects)
    {
        if (object->IsStarted()) continue;
        object->Start();
    }
}

void Scene::Update(float delta_time)
{
    for (auto& object : objects)
    {
        object->Update(delta_time);
    }
}

void Scene::LateUpdate()
{
    shared_ptr<Camera> test = Camera::GetMainCamera();
    Camera::GetMainCamera()->LateUpdate();
}

void Scene::Render()
{
    for (auto& object : objects)
    {
        object->Render();
    }
}

void Scene::ProcessAddAndDestroyObjects()
{
    for (auto& object : destroy_requested_objects)
    {
        objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    }

    destroy_requested_objects.clear();


    for (auto& object : add_requested_objects)
    {
        objects.push_back(object);
    }

    add_requested_objects.clear();
}
