#include "pch.h"
#include "Object.h"
#include "Frost.h"
#include "RenderFramework.h"


Frost::Frost()
{
	CreateGeometry();
	CreateVS();
	CreateGS();
	CreatePS();
	CreateInputLayout();
}

Frost::~Frost()
{
}

void Frost::Update()
{
}

void Frost::Render()
{
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	auto deviceContext = RenderFramework::GetDeviceContext();

	// IA
	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(_inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// VS
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

	// GS
	deviceContext->GSSetShader(_geometryShader.Get(), nullptr, 0);

	// RS

	// PS
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	// OM
	deviceContext->Draw(_vertices.size(), 0);

	// Cleanup
	//deviceContext->GSSetShader(nullptr, nullptr, 0);
}

void Frost::CreateGeometry()
{
	_vertices.resize(1);

	_vertices[0].position = Vec3(0, 0, 0);
	_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = _vertices.data();

	auto device = RenderFramework::GetDevice();
	device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
}

void Frost::CreateVS()
{
	LoadShaderFromFile(L"Point.hlsl", "VS", "vs_5_0", _vsBlob);

	auto device = RenderFramework::GetDevice();
	HRESULT hr = device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Frost::CreateGS()
{
	LoadShaderFromFile(L"Point.hlsl", "GS", "gs_5_0", _gsBlob);

	auto device = RenderFramework::GetDevice();
	HRESULT hr = device->CreateGeometryShader(_gsBlob->GetBufferPointer(), _gsBlob->GetBufferSize(), nullptr, _geometryShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Frost::CreatePS()
{
	LoadShaderFromFile(L"Point.hlsl", "PS", "ps_5_0", _psBlob);

	auto device = RenderFramework::GetDevice();
	HRESULT hr = device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Frost::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	auto device = RenderFramework::GetDevice();
	device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}