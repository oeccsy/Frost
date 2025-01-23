#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Camera.h"

void Scene::Update()
{
	for (const auto object : _objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	_camera->LateUpdate();
}

void Scene::Render(shared_ptr<Renderer> renderer)
{
	for (const auto object : _objects)
	{
		object->Render(renderer);
	}
}
