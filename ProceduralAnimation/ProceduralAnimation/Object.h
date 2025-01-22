#pragma once

class Mesh;
class Material;
class Transform;

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(shared_ptr<Pipeline> pipeline);

	shared_ptr<Transform> GetTransform() { return _transform; }

protected:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

protected:
	shared_ptr<Transform> _transform;

	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};