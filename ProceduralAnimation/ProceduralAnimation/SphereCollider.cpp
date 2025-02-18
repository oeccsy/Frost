#include "pch.h"
#include "Object.h"
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "SphereCollider.h"


SphereCollider::SphereCollider() : Collider(ColliderType::Sphere)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update()
{
	_boundingSphere.Center = GetOwner()->GetTransform()->GetWorldPosition();
	
	Vector3 scale = GetOwner()->GetTransform()->GetWorldScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Ray& ray, OUT float& distance)
{
	return _boundingSphere.Intersects(ray.position, ray.direction, OUT distance);
}

bool SphereCollider::Intersects(shared_ptr<Collider>& other)
{
	ColliderType otherType = other->GetColliderType();
	
	switch (otherType)
	{
	case ColliderType::Sphere:
		//return _boudningSPhere.Intersects(dynamic_pointer_case<SphereCollider>(other)->GetBoundingSphere());
		break;
	case ColliderType::AABB:
		break;
	case ColliderType::OBB:
		break;
	}

	return false;
}
