#include "pch.h"
#include "Mesh.h"
#include "Material.h"

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::CreateBuffers()
{
	auto device = Graphics::GetDevice();
	
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
}

void Mesh::CreateInputLayout(shared_ptr<Material> material)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
	   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	   {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HRESULT hr = Graphics::GetDevice()->CreateInputLayout(layout, count, material->_vsBlob->GetBufferPointer(), material->_vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}
