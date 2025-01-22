#pragma once

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	Camera,
	Animator,
	// ...
	Script,
	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1
};

class Object;
class Transform;

class Component
{
public:
	Component();
	Component(ComponentType type);
	virtual ~Component();

public:
	virtual void Update() {}
	virtual void LateUpdate() {}

public:
	shared_ptr<Object> GetOwner();
	shared_ptr<Transform> GetTransform();

private:
	friend class Object;
	void SetOwner(shared_ptr<Object> owner) { _owner = owner; }

protected:
	ComponentType _type;
	weak_ptr<Object> _owner;
};