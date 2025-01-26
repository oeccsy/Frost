#pragma once

class Mesh;
class Material;
class Transform;
class Component;

class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

public:
	virtual void Update();
	virtual void Render(shared_ptr<Renderer> renderer);

public:
	shared_ptr<Transform> GetTransform() { return _transform; }
	vector<shared_ptr<Component>>& GetComponents() { return _components; }
	void AddComponent(shared_ptr<Component> component);

protected:
	shared_ptr<Mesh> _mesh = make_shared<Mesh>();
	shared_ptr<Material> _material = make_shared<Material>();
	shared_ptr<Transform> _transform = make_shared<Transform>();

protected:
	vector<shared_ptr<Component>> _components;
};