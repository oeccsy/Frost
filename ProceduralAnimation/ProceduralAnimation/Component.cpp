#include "pch.h"
#include "Component.h"

Component::Component() {}

Component::Component(ComponentType type) { _type = type; }

Component::~Component() {}

shared_ptr<Object> Component::GetOwner()
{
	return _owner.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
	//return _owner.lock()->GetTransform();
	return nullptr;
}
