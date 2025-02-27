#pragma once

class Mesh;
class Material;
class Transform;
class Component;
class Collider;

class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

public:
	virtual void Update();
	virtual void Render(shared_ptr<Renderer> renderer);

public:
	shared_ptr<Mesh> GetMesh() { return _mesh; }
	shared_ptr<Material> GetMaterial() { return _material; }
	shared_ptr<Transform> GetTransform() { return _transform; }
	void AddComponent(shared_ptr<Component> component);

	template <typename T>
	shared_ptr<T> GetComponent()
	{
		for (const auto& comp : _components)
		{
			auto component = dynamic_pointer_cast<T>(comp);
			if (component) return component;
		}

		return nullptr;
	}

protected:
	shared_ptr<Mesh> _mesh = make_shared<Mesh>();
	shared_ptr<Material> _material = make_shared<Material>();
	shared_ptr<Transform> _transform = make_shared<Transform>();

protected:
	vector<shared_ptr<Component>> _components;
};