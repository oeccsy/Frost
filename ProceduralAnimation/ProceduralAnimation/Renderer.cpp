#include "pch.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"

ComPtr<ID3D11Buffer> Renderer::_constantBuffer = nullptr;

ComPtr<ID3D11Buffer> Renderer::_vertexBuffer = nullptr;
ComPtr<ID3D11Buffer> Renderer::_indexBuffer = nullptr;
ComPtr<ID3D11InputLayout> Renderer::_inputLayout = nullptr;
D3D11_PRIMITIVE_TOPOLOGY Renderer::_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

ComPtr<ID3D11VertexShader> Renderer::_vertexShader = nullptr;
ComPtr<ID3D11GeometryShader> Renderer::_geometryShader = nullptr;
ComPtr<ID3D11RasterizerState> Renderer::_rasterizerState = nullptr;
ComPtr<ID3D11PixelShader> Renderer::_pixelShader = nullptr;
vector<ComPtr<ID3D11ShaderResourceView>> Renderer::_textures;
ComPtr<ID3D11SamplerState> Renderer::_samplerState = nullptr;
ComPtr<ID3D11BlendState> Renderer::_blendState = nullptr;
ComPtr<ID3D11DepthStencilState> Renderer::_depthState = nullptr;

Renderer::Renderer()
{
	CreateConstantBuffer();
}

Renderer::~Renderer() {}

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

void Renderer::Bind(shared_ptr<Mesh> mesh)
{
	_vertexBuffer = mesh->_vertexBuffer;
	_indexBuffer = mesh->_indexBuffer;
	_inputLayout = mesh->_inputLayout;
	_topology = mesh->_topology;
}

void Renderer::Bind(shared_ptr<Material> material)
{
	_vertexShader = material->_vertexShader;
	_geometryShader = material->_geometryShader;
	_rasterizerState = material->_rasterizerState;
	_pixelShader = material->_pixelShader;
	_textures = material->_textures;
	_samplerState = material->_samplerState;
	_blendState = material->_blendState;

	D3D11_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	desc.StencilEnable = false;

	Graphics::GetDevice()->CreateDepthStencilState(&desc, _depthState.GetAddressOf());
}

void Renderer::Bind(shared_ptr<Transform> transform)
{
	TransformData transformData;
	transformData.worldMatrix = transform->GetWorldMatrix();
	transformData.viewMatrix = Camera::GetMainCamera()->GetViewMatrix();
	transformData.projMatrix = Camera::GetMainCamera()->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	auto deviceContext = Graphics::GetDeviceContext();
	deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &transformData, sizeof(transformData));
	deviceContext->Unmap(_constantBuffer.Get(), 0);
}