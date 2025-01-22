#include "pch.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

Object::Object()
{
	_mesh = make_shared<Mesh>();
	_material = make_shared<Material>();
	_transform = make_shared<Transform>();
}

Object::~Object() {}

void Object::Update()
{
	//// SRT
	//Matrix s = XMMatrixScalingFromVector(_localScale);
	//Matrix r = XMMatrixRotationRollPitchYawFromVector(_localRotation);
	//Matrix t = XMMatrixTranslationFromVector(_localPosition);

	//Matrix matWorld = s * r * t;
	//_transformData.matWorld = matWorld;

	//D3D11_MAPPED_SUBRESOURCE subResource;
	//ZeroMemory(&subResource, sizeof(subResource));

	//auto deviceContext = Graphics::GetDeviceContext();
	//deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	//deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Object::LateUpdate()
{
	_transform->CalculateMatrix();
}

void Object::Render(shared_ptr<Pipeline> pipeline)
{
	pipeline->Render(_mesh, _material);
}
