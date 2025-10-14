#pragma once

class Mesh;
class Material;
class Transform;
class Camera;

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

public:
	virtual void Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material, shared_ptr<Transform> transform) = 0;

protected:
	void CreateConstantBuffer();
	void Bind(shared_ptr<Mesh> mesh);
	void Bind(shared_ptr<Material> material);
	void Bind(shared_ptr<Transform> transform);

protected:
	static ComPtr<ID3D11Buffer> _constantBuffer;

	static ComPtr<ID3D11Buffer> _vertexBuffer;
	static ComPtr<ID3D11Buffer> _indexBuffer;
	static ComPtr<ID3D11InputLayout> _inputLayout;
	static D3D11_PRIMITIVE_TOPOLOGY _topology;

	static ComPtr<ID3D11VertexShader> _vertexShader;
	static ComPtr<ID3D11GeometryShader> _geometryShader;
	static ComPtr<ID3D11RasterizerState> _rasterizerState;
	static ComPtr<ID3D11PixelShader> _pixelShader;
	static vector<ComPtr<ID3D11ShaderResourceView>> _textures;
	static ComPtr<ID3D11SamplerState> _samplerState;
	static ComPtr<ID3D11BlendState> _blendState;
	static ComPtr<ID3D11DepthStencilState> _depthState;
};