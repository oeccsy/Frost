#pragma once
#include<vector>
#include<string>

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update();
	virtual void Render(shared_ptr<Pipeline> pipeline);

protected:
	virtual void CreateGeometry() = 0;
	virtual void CreateVS() = 0;
	virtual void CreatePS() = 0;
	virtual void CreateInputLayout() = 0;
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

protected:
	// IA
	vector<Vertex> _vertices;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;
	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11SamplerState> _samplerState;

	// OM
	ComPtr<ID3D11BlendState> _blendState;

protected:
	// Transform
	Vector _localPosition = { 0.f, 0.f, 0.f };
	Vector _localRotation = { 0.f, 0.f, 0.f };
	Vector _localScale = { 1.f, 1.f, 1.f };

	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};