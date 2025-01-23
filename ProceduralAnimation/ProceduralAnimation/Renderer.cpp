#include "pch.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"

Renderer::Renderer()
{
	CreateConstantBuffer();
}

Renderer::~Renderer() {}

void Renderer::Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material, shared_ptr<Transform> transform)
{
	BindConstantBuffer(transform);

	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	auto deviceContext = Graphics::GetDeviceContext();

	// IA	
	deviceContext->IASetVertexBuffers(0, 1, mesh->_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mesh->_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(mesh->_inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(mesh->_topology);

	// VS
	deviceContext->VSSetShader(material->_vertexShader.Get(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	
	// GS
	deviceContext->GSSetShader(material->_geometryShader.Get(), nullptr, 0);
	
	// RS
	deviceContext->RSSetState(material->_rasterizerState.Get());

	// PS
	deviceContext->PSSetShader(material->_pixelShader.Get(), nullptr, 0);
	UINT slot = 0;
	for (const auto& shaderResourceView : material->_textures)
	{
		deviceContext->PSSetShaderResources(slot++, 1, shaderResourceView.GetAddressOf());
	}

	// OM
	deviceContext->OMSetBlendState(material->_blendState.Get(), 0, 0xFFFFFFFF);


	deviceContext->DrawIndexed(static_cast<UINT>(mesh->_indices.size()), 0, 0);
}

void Renderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;				// CPU Write + GPU Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = Graphics::GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Renderer::BindConstantBuffer(shared_ptr<Transform> transform)
{
	TransformData newData;
	newData.worldMatrix = transform->GetWorldMatrix();
	newData.viewMatrix = Camera::GetMainCamera()->GetViewMatrix();
	newData.projMatrix = Camera::GetMainCamera()->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));
	
	auto deviceContext = Graphics::GetDeviceContext();
	deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &newData, sizeof(newData));
	deviceContext->Unmap(_constantBuffer.Get(), 0);
}
