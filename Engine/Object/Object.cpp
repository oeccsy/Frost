#include "Object.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Transform.h"
#include "Component/Component.h"
#include "Render/Renderer/Renderer.h"

Object::Object() { }

Object::~Object() {}

void Object::Awake()
{
    mesh = make_shared<Mesh>();
    material = make_shared<Material>();
    transform = make_shared<Transform>();
    
    is_awake = true;
}

void Object::Start()
{
    is_started = true;
}

void Object::Update(float delta_time)
{
    for (auto component : GetComponents())
    {
        component->Update();
    }
}

void Object::Render()
{
    shared_ptr<Renderer> renderer = GetComponent<Renderer>();
    if (renderer) renderer->Render();
}

void Object::AddComponent(shared_ptr<Component> component)
{
    components.push_back(component);
    component->SetOwner(shared_from_this());
    component->Awake();
}
