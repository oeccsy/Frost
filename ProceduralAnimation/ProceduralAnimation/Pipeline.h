#pragma once

struct PipelineInfo
{
	uint32 stride;
	uint32 offset;
	uint32 count;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11RasterizerState> rasterizerState;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11SamplerState> samplerState;
	ComPtr<ID3D11BlendState> blendState;
};

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);
	void SetVertexBuffer(ComPtr<ID3D11Buffer> buffer, uint32 s, uint32 o);
	void SetIndexBuffer(ComPtr<ID3D11Buffer> buffer);
};