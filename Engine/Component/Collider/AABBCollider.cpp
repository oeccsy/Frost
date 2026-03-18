#include "AABBCollider.h"
#include "Enum/ColliderType.h"
#include "Component/Collider/OBBCollider.h"
#include "Component/Collider/SphereCollider.h"

AABBCollider::AABBCollider() : Collider(ColliderType::AABB) { }

AABBCollider::~AABBCollider() {}


bool AABBCollider::Intersects(Ray& ray, OUT float& distance)
{
	return bounding_box.Intersects(ray.position, ray.direction, OUT distance);
}

bool AABBCollider::Intersects(shared_ptr<Collider>& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return bounding_box.Intersects(other->As<SphereCollider>()->GetBoundingSphere());
	case ColliderType::AABB:
		return bounding_box.Intersects(other->As<AABBCollider>()->GetBoundingBox());
	case ColliderType::OBB:
		return bounding_box.Intersects(other->As<OBBCollider>()->GetBoundingBox());
	default:
		return false;
	}
}
