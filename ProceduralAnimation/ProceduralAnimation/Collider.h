#pragma once

enum class ColliderType
{
	Sphere,
	AABB,
	OBB,
	Mesh
};

class Collider : public Component
{
public:
	Collider(ColliderType colliderType);
	virtual ~Collider();

public:
	virtual bool Intersects(Ray& ray, OUT float& distanct) = 0;
	virtual bool Intersects(shared_ptr<Collider>& other) = 0;

	ColliderType GetColliderType() { return _colliderType; }

protected:
	ColliderType _colliderType;
};