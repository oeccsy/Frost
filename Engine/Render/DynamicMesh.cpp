#include "DynamicMesh.h"
#include "Graphics.h"

DynamicMesh::DynamicMesh() { }

DynamicMesh::~DynamicMesh() { }

void DynamicMesh::CreateBuffers()
{
	auto device = Graphics::GetDevice();
	
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * 200); // TODO : ũ�� �ʰ� ����
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, vertex_buffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * 200);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, index_buffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
}

void DynamicMesh::UpdateBuffers()
{
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		auto deviceContext = Graphics::GetDeviceContext();
		deviceContext->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, vertices.data(), sizeof(Vertex) * vertices.size());
		deviceContext->Unmap(vertex_buffer.Get(), 0);
	}

	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		auto deviceContext = Graphics::GetDeviceContext();
		deviceContext->Map(index_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, indices.data(), sizeof(uint32) * indices.size());
		deviceContext->Unmap(index_buffer.Get(), 0);
	}
}

