#include "pch.h"
#include "Renderer.h"
#include "BasicRenderer.h"
#include "Mesh.h"

BasicRenderer::BasicRenderer() {}

BasicRenderer::~BasicRenderer() {}

void BasicRenderer::Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material, shared_ptr<Transform> transform)
{
	Bind(mesh);
	Bind(material);
	Bind(transform);

	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	auto deviceContext = Graphics::GetDeviceContext();

	// IA	
	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(_inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(_topology);

	// VS
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	// GS
	deviceContext->GSSetShader(_geometryShader.Get(), nullptr, 0);

	// RS
	deviceContext->RSSetState(_rasterizerState.Get());

	// PS
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
	UINT slot = 0;
	for (const auto& shaderResourceView : _textures)
	{
		deviceContext->PSSetShaderResources(slot++, 1, shaderResourceView.GetAddressOf());
	}

	// OM
	deviceContext->OMSetBlendState(_blendState.Get(), 0, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(_depthState.Get(), 0);


	deviceContext->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0, 0);
}
