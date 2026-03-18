#pragma once

#include "Core.h"
#include "Component/Component.h"

class ENGINE_API Renderer : public Component
{
	RTTI_DECLARATIONS(Renderer, Component)

public:
	Renderer();
	virtual ~Renderer();

	virtual void Render();

protected:
	void CreateConstantBuffer();
	void Bind(shared_ptr<class Mesh> mesh);
	void Bind(shared_ptr<class Material> material);
	void Bind(shared_ptr<class Transform> transform);

protected:
	ComPtr<ID3D11Buffer> constant_buffer;

	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;
	ComPtr<ID3D11InputLayout> input_layout;
	D3D11_PRIMITIVE_TOPOLOGY topology;

	ComPtr<ID3D11VertexShader> vertex_shader;
	ComPtr<ID3D11GeometryShader> geometry_shader;
	ComPtr<ID3D11RasterizerState> rasterizer_state;
	ComPtr<ID3D11PixelShader> pixel_shader;
	vector<ComPtr<ID3D11ShaderResourceView>> textures;
	ComPtr<ID3D11SamplerState> sampler_state;
	ComPtr<ID3D11BlendState> blend_state;
};