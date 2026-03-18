#include "pch.h"
#include "Material.h"
#include "Mesh.h"
#include "DynamicMesh.h"
#include "BufferPool.h"

DynamicMesh::DynamicMesh() {}

DynamicMesh::~DynamicMesh()
{
	if (_useVertexBufferPool) BufferPool::vertexBufferPool->ReturnBuffer(_vertexBuffer);
	if (_useIndexBufferPool) BufferPool::indexBufferPool->ReturnBuffer(_indexBuffer);
}

void DynamicMesh::CreateBuffers()
{
	auto device = Graphics::GetDevice();

	if (BufferPool::vertexBufferPool != nullptr)
	{
		_vertexBuffer = BufferPool::vertexBufferPool->GetBuffer();
		_useVertexBufferPool = true;
	}
	else
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * 200); // TODO : 크기 초과 대응
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	if (BufferPool::indexBufferPool != nullptr)
	{
		_indexBuffer = BufferPool::indexBufferPool->GetBuffer();
		_useIndexBufferPool = true;
	}
	else
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * 200);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
}

void DynamicMesh::UpdateBuffers()
{
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		auto deviceContext = Graphics::GetDeviceContext();
		deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, _vertices.data(), sizeof(Vertex) * _vertices.size());
		deviceContext->Unmap(_vertexBuffer.Get(), 0);
	}

	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		auto deviceContext = Graphics::GetDeviceContext();
		deviceContext->Map(_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, _indices.data(), sizeof(uint32) * _indices.size());
		deviceContext->Unmap(_indexBuffer.Get(), 0);
	}
}

