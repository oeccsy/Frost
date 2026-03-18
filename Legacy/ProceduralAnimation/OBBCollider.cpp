#include "pch.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

OBBCollider::~OBBCollider() {}

void OBBCollider::Update() {}

bool OBBCollider::Intersects(Ray& ray, OUT float& distance)
{
	return _boundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

bool OBBCollider::Intersects(shared_ptr<Collider>& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return _boundingBox.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
	case ColliderType::AABB:
		return _boundingBox.Intersects(dynamic_pointer_cast<AABBCollider>(other)->GetBoundingBox());
	case ColliderType::OBB:
		return _boundingBox.Intersects(dynamic_pointer_cast<OBBCollider>(other)->GetBoundingBox());
	}

	return false;
}
