#include "pch.h"
#include "Material.h"

Material::Material() {}

Material::~Material() {}

void Material::CreateVS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, _vsBlob);
	HRESULT hr = Graphics::GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::CreateGS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, _gsBlob);
	HRESULT hr = Graphics::GetDevice()->CreateGeometryShader(_gsBlob->GetBufferPointer(), _gsBlob->GetBufferSize(), nullptr, _geometryShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::CreatePS(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version, _psBlob);
	HRESULT hr = Graphics::GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Material::AddVSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer)
{
	_vsConstantBuffers.push_back(constantBuffer);
}

void Material::AddGSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer)
{
	_gsConstantBuffers.push_back(constantBuffer);
}

void Material::AddPSConstantBuffer(ComPtr<ID3D11Buffer>& constantBuffer)
{
	_psConstantBuffers.push_back(constantBuffer);
}

void Material::AddTexture(const wstring& path)
{
	TexMetadata md;
	ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	assert(SUCCEEDED(hr));

	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	auto device = Graphics::GetDevice();
	hr = CreateShaderResourceView(device.Get(), img.GetImages(), img.GetImageCount(), md, shaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));

	_textures.push_back(shaderResourceView);
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

