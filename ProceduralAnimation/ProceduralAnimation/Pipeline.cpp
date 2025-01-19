#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	auto deviceContext = Graphics::GetDeviceContext();
	
	// IA	
	deviceContext->IASetVertexBuffers(0, 1, info.vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(info.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(info.inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(info.topology);

	// VS
	deviceContext->VSSetShader(info.vertexShader.Get(), nullptr, 0);
	// deviceContext->VSSetConstantBuffers(0, 1, info.constantBuffer.GetAddressOf());
	
	// RS
	deviceContext->RSSetState(info.rasterizerState.Get());

	// PS
	deviceContext->PSSetShader(info.pixelShader.Get(), nullptr, 0);
	// deviceContext->PSSetShaderResources(0, 1, _shaderResourceView_smile.GetAddressOf());

	// OM
	deviceContext->OMSetBlendState(info.blendState.Get(), 0, 0xFFFFFFFF);
	deviceContext->DrawIndexed(info.count, 0, 0);
}

void Pipeline::SetVertexBuffer(ComPtr<ID3D11Buffer> buffer, uint32 s, uint32 o)
{
	uint32 stride = s;
	uint32 offset = o;

	auto deviceContext = Graphics::GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(ComPtr<ID3D11Buffer> buffer)
{
	auto deviceContext = Graphics::GetDeviceContext();
	deviceContext->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}