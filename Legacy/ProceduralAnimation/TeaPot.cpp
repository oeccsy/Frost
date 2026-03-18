#include "pch.h"
#include "Object.h"
#include "TeaPot.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"

TeaPot::TeaPot()
{
	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

TeaPot::~TeaPot() { }

void TeaPot::CreateGeometry()
{
	ResourceManager::LoadObj("teapot.obj", _mesh);
}

void TeaPot::CreateShader()
{
	_material->CreateVS(L"Phong.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Phong.hlsl", "PS", "ps_5_0");
}

void TeaPot::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}
