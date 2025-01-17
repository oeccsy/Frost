#include "pch.h"
#include "Scene.h"
#include "Object.h"

void Scene::Update()
{
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene::Render()
{
	for (auto object : objects)
	{
		object->Render();
	}
}