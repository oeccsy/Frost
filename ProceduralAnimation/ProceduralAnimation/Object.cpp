#include "pch.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

Object::Object()
{
	_mesh = make_shared<Mesh>();
	_material = make_shared<Material>();
	_transform = make_shared<Transform>();
}

Object::~Object() {}

void Object::Update() {}

void Object::LateUpdate() {}

void Object::Render(shared_ptr<Renderer> renderer)
{
	renderer->Render(_mesh, _material, _transform);
}
