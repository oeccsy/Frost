#include "Object.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Transform.h"
#include "Component/Component.h"

Object::Object()
{
    mesh = make_shared<Mesh>();
    material = make_shared<Material>();
    transform = make_shared<Transform>();
}

Object::~Object() {}

void Object::Awake() {}

void Object::Update(float delta_time)
{
    for (auto component : GetComponents())
    {
        component->Update();
    }
}

void Object::Render() { }

void Object::AddComponent(shared_ptr<Component> component)
{
    components.push_back(component);
    component->SetOwner(shared_from_this());
    component->Awake();
}
