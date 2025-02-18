#include "pch.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

AABBCollider::~AABBCollider() {}

void AABBCollider::Update() {}

bool AABBCollider::Intersects(Ray& ray, OUT float& distance)
{
	return _boundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

bool AABBCollider::Intersects(shared_ptr<Collider>& other)
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
	default:
		return false;
	}
}
