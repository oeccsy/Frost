#include "Material.h"
#include "Graphics.h"
#include "Types.h"

Material::Material()
{
	CreateRasterizerState();
}

Material::~Material() {}

unordered_map<size_t, shared_ptr<Material>> Material::materials;

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

void Material::LoadShaderFromFile(const wstring& path, const string& entry_point, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // TODO : Debug -> Release

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry_point.c_str(),
		version.c_str(),
		compile_flag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	assert(SUCCEEDED(hr));
}

void Material::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = true;

	HRESULT hr = Graphics::GetDevice()->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
	assert(SUCCEEDED(hr));
}

