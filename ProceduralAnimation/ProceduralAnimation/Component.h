#pragma once

class Object;

class Component
{
public:
	Component();
	virtual ~Component();

public:
	virtual void Init() {}
	virtual void Update() {}
	virtual void LateUpdate() {}

	virtual void Render(shared_ptr<Renderer> renderer) {}

public:
	shared_ptr<Object> GetOwner() { return _owner.lock(); };

private:
	friend class Object;
	void SetOwner(shared_ptr<Object> owner) { _owner = owner; }

protected:
	weak_ptr<Object> _owner;
};