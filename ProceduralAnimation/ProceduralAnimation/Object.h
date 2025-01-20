#pragma once

class Mesh;
class Material;

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update();
	virtual void Render(shared_ptr<Pipeline> pipeline);

protected:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

protected:
	// Transform
	Vector _localPosition = { 0.f, 0.f, 0.f };
	Vector _localRotation = { 0.f, 0.f, 0.f };
	Vector _localScale = { 1.f, 1.f, 1.f };

	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};