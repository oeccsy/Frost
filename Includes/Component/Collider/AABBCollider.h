#pragma once

#include "Core.h"
#include "Component/Collider//Collider.h"
#include "RTTI.h"
#include "Types.h"

class ENGINE_API AABBCollider : Collider
{
	RTTI_DECLARATIONS(AABBCollider, Collider)

public:
	AABBCollider();
	virtual ~AABBCollider() override;

public:
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

	FORCEINLINE const BoundingBox& GetBoundingBox() const { return bounding_box; }

private:
	BoundingBox bounding_box;
};