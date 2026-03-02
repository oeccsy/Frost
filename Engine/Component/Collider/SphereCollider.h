#pragma once

#include "Core.h"
#include "Component/Collider/Collider.h"
#include "RTTI.h"
#include "Types.h"

class ENGINE_API SphereCollider : public Collider
{
	RTTI_DECLARATIONS(SphereCollider, Collider);

public:
	SphereCollider();
	virtual ~SphereCollider() override;

public:
	virtual void Update() override;
	virtual bool Intersects(Ray&, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

	FORCEINLINE const BoundingSphere& GetBoundingSphere() const { return bounding_sphere; }
	FORCEINLINE void SetRadius(float new_radius) { radius = new_radius; }

private:
	BoundingSphere bounding_sphere;
	float radius = 1.0f;
};