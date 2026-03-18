#include "Renderer.h"
#include "Object/Object.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Transform.h"
#include "Graphics.h"
#include "Render/Camera.h"
#include "Data/TransformData.h"

Renderer::Renderer()
{
	constant_buffer = nullptr;
	vertex_buffer = nullptr;
	index_buffer = nullptr;
	input_layout = nullptr;
	topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	vertex_shader = nullptr;
	geometry_shader = nullptr;
	rasterizer_state = nullptr;
	pixel_shader = nullptr;
	sampler_state = nullptr;
	blend_state = nullptr;
	
	CreateConstantBuffer();
}

Renderer::~Renderer() {}

void Renderer::Render()
{
	shared_ptr<Mesh> mesh = GetOwner()->GetMesh();
	shared_ptr<Material> material = GetOwner()->GetMaterial();
	shared_ptr<Transform> transform = GetOwner()->GetTransform();
	
	Bind(mesh);
	Bind(material);
	Bind(transform);
	
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;
	
	auto device_context = Graphics::GetDeviceContext();
	
	// IA
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	device_context->IASetInputLayout(input_layout.Get());
	device_context->IASetPrimitiveTopology(topology);
	
	// VS
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	
	// GS
	device_context->GSSetShader(geometry_shader.Get(), nullptr, 0);
	
	// RS
	device_context->RSSetState(rasterizer_state.Get());
	
	// PS
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	UINT slot = 0;
	for (const auto& shader_resource_view : textures)
	{
		device_context->PSSetShaderResources(slot++, 1, shader_resource_view.GetAddressOf());
	}
	
	// OM
	device_context->OMSetBlendState(blend_state.Get(), 0, 0xFFFFFFFF);
	
	device_context->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0, 0);
}

void Renderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;				// CPU Write + GPU Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = Graphics::GetDevice()->CreateBuffer(&desc, nullptr, constant_buffer.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Renderer::Bind(shared_ptr<Mesh> mesh)
{
	vertex_buffer = mesh->vertex_buffer;
	index_buffer = mesh->index_buffer;
	input_layout = mesh->input_layout;
	topology = mesh->topology;
}

void Renderer::Bind(shared_ptr<Material> material)
{
	vertex_shader = material->vertex_shader;
	geometry_shader = material->geometry_shader;
	rasterizer_state = material->rasterizer_state;
	pixel_shader = material->pixel_shader;
	textures = material->textures;
	sampler_state = material->sampler_state;
	blend_state = material->blend_state;
}

void Renderer::Bind(shared_ptr<Transform> transform)
{
	TransformData transform_data;
	transform_data.world_matrix = transform->GetWorldMatrix();
	transform_data.view_matrix = Camera::GetMainCamera()->GetViewMatrix();
	transform_data.proj_matrix = Camera::GetMainCamera()->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE sub_resource;
	ZeroMemory(&sub_resource, sizeof(sub_resource));

	auto device_context = Graphics::GetDeviceContext();
	device_context->Map(constant_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub_resource);
	::memcpy(sub_resource.pData, &transform_data, sizeof(transform_data));
	device_context->Unmap(constant_buffer.Get(), 0);
}