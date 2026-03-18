#include "SphereCollider.h"
#include "Enum/ColliderType.h"
#include "Object/Object.h"
#include "Render/Transform.h"
#include "Component/Collider/AABBCollider.h"
#include "Component/Collider/OBBCollider.h"

SphereCollider::SphereCollider() : Collider(ColliderType::Sphere) {}

SphereCollider::~SphereCollider() {}

void SphereCollider::Update()
{
	Vector3 owner_position = GetOwner()->GetTransform()->GetWorldPosition();
	Vector3 owner_scale = GetOwner()->GetTransform()->GetWorldScale();

	bounding_sphere.Center = owner_position;
	bounding_sphere.Radius = radius * max(max(owner_scale.x, owner_scale.y), owner_scale.z);
}

bool SphereCollider::Intersects(Ray& ray, OUT float& distance)
{
	return bounding_sphere.Intersects(ray.position, ray.direction, OUT distance);
}

bool SphereCollider::Intersects(shared_ptr<Collider>& other)
{
	ColliderType other_type = other->GetColliderType();

	switch (other_type)
	{
	case ColliderType::Sphere:
		return bounding_sphere.Intersects(other->As<SphereCollider>()->GetBoundingSphere());
		break;
	case ColliderType::AABB:
		return bounding_sphere.Intersects(other->As<AABBCollider>()->GetBoundingBox());
		break;
	case ColliderType::OBB:
		return bounding_sphere.Intersects(other->As<OBBCollider>()->GetBoundingBox());
		break;
	}

	return false;
}
