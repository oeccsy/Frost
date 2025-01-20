#pragma once
#include <string>

class Material
{
public:
	Material();
	virtual ~Material();

	void CreateVS(const wstring& path, const string& name, const string& version);
	void CreateGS(const wstring& path, const string& name, const string& version);
	void CreatePS(const wstring& path, const string& name, const string& version);
	void AddVSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer);
	void AddGSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer);
	void AddPSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer);
	void AddTexture(const wstring& path);

	template<typename T>
	static ComPtr<ID3D11Buffer> CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;				// CPU Write + GPU Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ComPtr<ID3D11Buffer> constantBuffer;
		HRESULT hr = device->CreateBuffer(&desc, nullptr, constantBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));

		return constantBuffer;
	}

private:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	friend class Mesh;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3DBlob> _vsBlob;
	vector<ComPtr<ID3D11Buffer>> _vsConstantBuffers;

	// GS
	ComPtr<ID3D11GeometryShader> _geometryShader;
	ComPtr<ID3DBlob> _gsBlob;
	vector<ComPtr<ID3D11Buffer>> _gsConstantBuffers;

	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _psBlob;
	vector<ComPtr<ID3D11Buffer>> _psConstantBuffers;
	vector<ComPtr<ID3D11ShaderResourceView>> _textures;
	ComPtr<ID3D11SamplerState> _samplerState;

	// OM
	ComPtr<ID3D11BlendState> _blendState;
};