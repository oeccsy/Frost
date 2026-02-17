#pragma once

#include "Core.h"
#include <memory>

class ENGINE_API Component
{
public:
	Component();
	virtual ~Component();
	
	virtual void Awake();
	virtual void Update();
	virtual void LateUpdate();
	
	FORCEINLINE shared_ptr<class Object> GetOwner() const { return owner.lock(); };
	FORCEINLINE void SetOwner(shared_ptr<class Object> new_owner) { owner = new_owner; }

protected:
	weak_ptr<class Object> owner;
};