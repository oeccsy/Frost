#include "pch.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"

Collider::Collider(ColliderType colliderType)
{
	_colliderType = colliderType;
}

Collider::~Collider() {}
