#include "pch.h"
#include "Pipeline.h"
#include "Mesh.h";
#include "Material.h";

Pipeline::Pipeline() {}

Pipeline::~Pipeline() {}

void Pipeline::Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material)
{
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
	UINT slot = 0;
	for (const auto& buffer : material->_vsConstantBuffers)
	{
		deviceContext->VSSetConstantBuffers(slot++, 1, buffer.GetAddressOf());
	}
	
	// GS
	deviceContext->GSSetShader(material->_geometryShader.Get(), nullptr, 0);
	
	// RS
	deviceContext->RSSetState(material->_rasterizerState.Get());

	// PS
	deviceContext->PSSetShader(material->_pixelShader.Get(), nullptr, 0);
	slot = 0;
	for (const auto& shaderResourceView : material->_textures)
	{
		deviceContext->PSSetShaderResources(slot++, 1, shaderResourceView.GetAddressOf());
	}

	// OM
	deviceContext->OMSetBlendState(material->_blendState.Get(), 0, 0xFFFFFFFF);
	deviceContext->DrawIndexed(mesh->_indices.size(), 0, 0);
}