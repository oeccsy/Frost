#include "pch.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"

Object::Object() {}

Object::~Object() {}

void Object::Update()
{
	for (auto comp : GetComponents())
	{
		comp->Update();
	}
}

void Object::Render(shared_ptr<Renderer> renderer)
{
#ifdef RENDER_OBJECT
	renderer->Render(_mesh, _material, _transform);
#endif

	for (auto comp : GetComponents())
	{
		comp->Render(renderer);
	}
}

void Object::AddComponent(shared_ptr<Component> component)
{
	_components.push_back(component);
	component->SetOwner(shared_from_this());
	component->Init();
}
