#pragma once

#include "Core.h"
#include "Component/Component.h"
#include "RTTI.h"
#include "Enum/ColliderType.h"
#include "Types.h"

class ENGINE_API Collider : public Component
{
	RTTI_DECLARATIONS(Collider, Component);

public:
	Collider(ColliderType collider_type);
	virtual ~Collider();

public:
	virtual bool Intersects(Ray& ray, OUT float& distance) = 0;
	virtual bool Intersects(shared_ptr<Collider>& other) = 0;

	FORCEINLINE ColliderType GetColliderType() const { return collider_type; }

protected:
	ColliderType collider_type;
};