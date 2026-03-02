#include "pch.h"
#include "Teapot.h"
#include "Data/Vertex.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Renderer/Renderer.h"
#include "Utils/ResourceManager.h"

Teapot::Teapot() { }

Teapot::~Teapot() { }

void Teapot::Awake()
{
	Object::Awake();

	CreateGeometry();
	CreateShader();
	CreateInputLayout();

	// AddComponent(make_shared<Renderer>());
}

void Teapot::Update(float delta_time)
{
	Object::Update(delta_time);
}

void Teapot::CreateGeometry()
{
	ResourceManager::LoadObj("../Assets/teapot.obj", mesh);
}

void Teapot::CreateShader()
{
	GetMaterial()->CreateVS(L"Shader/Phong.hlsl", "VS", "vs_5_0");
	GetMaterial()->CreatePS(L"Shader/Phong.hlsl", "PS", "ps_5_0");
}

void Teapot::CreateInputLayout()
{
	GetMesh()->CreateInputLayout(GetMaterial());
}
