#include "pch.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

Object::Object() {}

Object::~Object() {}

void Object::Render(shared_ptr<Renderer> renderer)
{
	renderer->Render(_mesh, _material, _transform);
}
