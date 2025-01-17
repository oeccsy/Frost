#include "pch.h"
#include "Object.h"
#include "Triangle.h"
#include "RenderFramework.h"

Triangle::Triangle()
{
	CreateGeometry();
	CreateVS();
	CreatePS();
	CreateInputLayout();
}

Triangle::~Triangle()
{
}

void Triangle::Update()
{

}

void Triangle::Render()
{
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	auto deviceContext = RenderFramework::GetDeviceContext();

	// IA
	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(_inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

	// RS

	// PS
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	// OM
	deviceContext->Draw(_vertices.size(), 0);
}

void Triangle::CreateGeometry()
{
	_vertices.resize(3);

	_vertices[0].position = Vec3(-0.5f, -.5f, 0);
	_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
	_vertices[1].position = Vec3(0, 0.5f, 0);
	_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
	_vertices[2].position = Vec3(0.5f, -0.5f, 0);
	_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);


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

void Triangle::CreateVS()
{
	LoadShaderFromFile(L"Triangle.hlsl", "VS", "vs_5_0", _vsBlob);

	auto device = RenderFramework::GetDevice();
	HRESULT hr = device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Triangle::CreatePS()
{
	LoadShaderFromFile(L"Triangle.hlsl", "PS", "ps_5_0", _psBlob);

	auto device = RenderFramework::GetDevice();
	HRESULT hr = device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Triangle::CreateInputLayout()
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