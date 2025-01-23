#pragma once

class Mesh;
class Material;
class Transform;

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update() = 0;
	virtual void Render(shared_ptr<Renderer> renderer);

	shared_ptr<Transform> GetTransform() { return _transform; }

protected:
	shared_ptr<Mesh> _mesh = make_shared<Mesh>();
	shared_ptr<Material> _material = make_shared<Material>();
	shared_ptr<Transform> _transform = make_shared<Transform>();
};