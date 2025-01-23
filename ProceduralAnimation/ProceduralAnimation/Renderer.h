#pragma once

class Mesh;
class Material;
class Transform;
class Camera;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material, shared_ptr<Transform> transform);

private:
	void CreateConstantBuffer();
	void BindConstantBuffer(shared_ptr<Transform> transform);

private:
	shared_ptr<Camera> camera;
	ComPtr<ID3D11Buffer> _constantBuffer;
};