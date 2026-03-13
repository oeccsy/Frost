#include "pch.h"
#include "FrostVisual.h"
#include "Render/Mesh.h"
#include "Render/DynamicMesh.h"
#include "Render/Material.h"

FrostVisual::FrostVisual() { }

FrostVisual::~FrostVisual() { }

void FrostVisual::Awake()
{
	Object::Awake();

	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

void FrostVisual::Update(float delta_time)
{
	Object::Update(delta_time);
}

void FrostVisual::CreateGeometry()
{
	mesh = make_shared<DynamicMesh>();
	mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	mesh->CreateBuffers();
}

void FrostVisual::CreateShader()
{
	material->CreateVS(L"Shader/Frost.hlsl", "VS", "vs_5_0");
	material->CreatePS(L"Shader/Frost.hlsl", "PS", "ps_5_0");
}

void FrostVisual::CreateInputLayout()
{
	mesh->CreateInputLayout(material);
}
