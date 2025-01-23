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
	void AddTexture(const wstring& path);

private:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	friend class Mesh;
	friend class Renderer;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3DBlob> _vsBlob;

	// GS
	ComPtr<ID3D11GeometryShader> _geometryShader;
	ComPtr<ID3DBlob> _gsBlob;
	
	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _psBlob;
	vector<ComPtr<ID3D11ShaderResourceView>> _textures;
	ComPtr<ID3D11SamplerState> _samplerState;

	// OM
	ComPtr<ID3D11BlendState> _blendState;
};