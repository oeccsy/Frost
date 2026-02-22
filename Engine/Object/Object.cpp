#include "Object.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Transform.h"
#include "Component/Component.h"
#include "Render/Renderer/Renderer.h"

Object::Object()
{
    mesh = make_shared<Mesh>();
    material = make_shared<Material>();
    transform = make_shared<Transform>();
}

Object::~Object() {}

void Object::Awake()
{
    is_awake = true;
}

void Object::Start()
{
    is_started = true;
}

void Object::Update(float delta_time)
{
    for (const shared_ptr<Component>& component : GetComponents())
    {
        component->Update();
    }
}

void Object::Render()
{
    vector<shared_ptr<Renderer>> renderers = GetComponents<Renderer>();
    
    for (shared_ptr<Renderer>& renderer : renderers)
    {
        if (renderer) renderer->Render();
    }
}

void Object::AddComponent(shared_ptr<Component> component)
{
    components.push_back(component);
    component->SetOwner(shared_from_this());
    component->Awake();
}
