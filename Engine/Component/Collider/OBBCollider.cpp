#include "OBBCollider.h"
#include "Enum/ColliderType.h"
#include "Component/Collider/AABBCollider.h"
#include "Component/Collider/SphereCollider.h"

OBBCollider::OBBCollider() : Collider(ColliderType::OBB) { }

OBBCollider::~OBBCollider() {}

bool OBBCollider::Intersects(Ray& ray, OUT float& distance)
{
	return bounding_box.Intersects(ray.position, ray.direction, OUT distance);
}

bool OBBCollider::Intersects(shared_ptr<Collider>& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return bounding_box.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
	case ColliderType::AABB:
		return bounding_box.Intersects(dynamic_pointer_cast<AABBCollider>(other)->GetBoundingBox());
	case ColliderType::OBB:
		return bounding_box.Intersects(dynamic_pointer_cast<OBBCollider>(other)->GetBoundingBox());
	}

	return false;
}
