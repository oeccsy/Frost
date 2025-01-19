#include "pch.h"
#include "Object.h"

Object::Object()
{
	// CreateGeometry();
	// CreateRectangle();
	// CreateVertices();
	// CreateIndices();
	// CreateVS();
	// CreatePS();
	// CreateInputLayout();
	// CreateRasterizerState();
	// CreateBlendState();
	// CreateConstantBuffer();
	// CreateTexture();
	// CreateSamplerState();
}

Object::~Object() {}

void Object::Update()
{
	// SRT
	Matrix s = XMMatrixScalingFromVector(_localScale);
	Matrix r = XMMatrixRotationRollPitchYawFromVector(_localRotation);
	Matrix t = XMMatrixTranslationFromVector(_localPosition);

	Matrix matWorld = s * r * t;
	_transformData.matWorld = matWorld;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	//auto deviceContext = Graphics::GetDeviceContext();
	//deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	//deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Object::Render(shared_ptr<Pipeline> pipeline)
{
	{
		PipelineInfo info;
		info.stride = sizeof(Vertex);
		info.offset = 0;
		info.count = _indices.size();
		
		info.vertexBuffer = _vertexBuffer;
		info.indexBuffer = _indexBuffer;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.rasterizerState = _rasterizerState;
		info.pixelShader = _pixelShader;
		info.texture = _texture;
		info.samplerState = _samplerState;
		info.blendState = _blendState;

		pipeline->UpdatePipeline(info);
	}
}

void Object::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

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

