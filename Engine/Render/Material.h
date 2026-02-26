#pragma once

#include "Core.h"
#include <string>

class ENGINE_API Material
{
public:
	Material();
	virtual ~Material();

public:
	void CreateVS(const wstring& path, const string& name, const string& version);
	void CreateGS(const wstring& path, const string& name, const string& version);
	void CreatePS(const wstring& path, const string& name, const string& version);
	void AddTexture(const wstring& path);

private:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
	void CreateRasterizerState();

private:
	friend class Renderer;
	friend class Mesh;

	// VS
	ComPtr<ID3D11VertexShader> vertex_shader;
	ComPtr<ID3DBlob> vs_blob;

	// GS
	ComPtr<ID3D11GeometryShader> geometry_shader;
	ComPtr<ID3DBlob> gs_blob;

	// RS
	ComPtr<ID3D11RasterizerState> rasterizer_state;

	// PS
	ComPtr<ID3D11PixelShader> pixel_shader;
	ComPtr<ID3DBlob> ps_blob;

	vector<ComPtr<ID3D11ShaderResourceView>> textures;
	ComPtr<ID3D11SamplerState> sampler_state;

	// OM
	ComPtr<ID3D11BlendState> blend_state;
};