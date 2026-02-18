#include "Scene.h"
#include "Object/Object.h"
#include "Render/Camera.h"

Scene::Scene() { }

Scene::~Scene() { }

void Scene::AddObject(shared_ptr<class Object> new_object)
{
    add_requested_objects.emplace_back(new_object);
}

void Scene::DestroyObject(shared_ptr<class Object> destroyed_object)
{
    destroy_requested_objects.emplace_back(destroyed_object);
}

void Scene::Init() { }

void Scene::Update(float delta_time)
{
    for (auto object : objects)
    {
        object->Update(delta_time);
    }
}

void Scene::LateUpdate()
{
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
        objects.emplace_back(object);
    }

    add_requested_objects.clear();
}
