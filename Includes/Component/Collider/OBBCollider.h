#pragma once

#include "Core.h"
#include "Component/Collider/Collider.h"
#include "RTTI.h"
#include "Types.h"

class ENGINE_API OBBCollider : public Collider
{
	RTTI_DECLARATIONS(OBBCollider, Collider);
	
public:
	OBBCollider();
	virtual ~OBBCollider();

public:
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

	FORCEINLINE const BoundingOrientedBox& GetBoundingBox() const { return bounding_box; }

private:
	BoundingOrientedBox bounding_box;
};