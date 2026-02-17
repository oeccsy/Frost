#include "Material.h"
#include "Graphics.h"
#include "Types.h"

Material::Material() {}

Material::~Material() {}

void Material::CreateVS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, vs_blob);
	HRESULT hr = Graphics::GetDevice()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, vertex_shader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::CreateGS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, gs_blob);
	HRESULT hr = Graphics::GetDevice()->CreateGeometryShader(gs_blob->GetBufferPointer(), gs_blob->GetBufferSize(), nullptr, geometry_shader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::CreatePS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, ps_blob);
	HRESULT hr = Graphics::GetDevice()->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, pixel_shader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::AddTexture(const wstring& path)
{
	TexMetadata meta_data;
	ScratchImage image;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta_data, image);
	assert(SUCCEEDED(hr));

	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	auto device = Graphics::GetDevice();
	hr = CreateShaderResourceView(device.Get(), image.GetImages(), image.GetImageCount(), meta_data, shader_resource_view.GetAddressOf());
	assert(SUCCEEDED(hr));

	textures.push_back(shader_resource_view);
}

void Material::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // TODO : Debug -> Release

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	assert(SUCCEEDED(hr));
}

