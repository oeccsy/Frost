#include "pch.h"
#include "Triangle.h"
#include "Data/Vertex.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Renderer/Renderer.h"

Triangle::Triangle() { }

Triangle::~Triangle() { }

void Triangle::Awake()
{
    Object::Awake();
    
    CreateGeometry();
    CreateShader();
    CreateInputLayout();
}

void Triangle::Update(float delta_time) { }

void Triangle::CreateGeometry()
{
    vector<Vertex> vertices;
    vertices.resize(3);

    vertices[0].position = Vector3(0.5f, 0.5f, 0);
    vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
    vertices[1].position = Vector3(-0.5f, 0.5f, 0);
    vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
    vertices[2].position = Vector3(-0.5f, -0.5f, 0);
    vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);

    vector<uint32> indices;
    indices = { 0, 1, 2 };

    GetMesh()->SetVertices(vertices);
    GetMesh()->SetIndices(indices);

    GetMesh()->CreateBuffers();
    GetMesh()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Triangle::CreateShader()
{
    GetMaterial()->CreateVS(L"Shader/Triangle.hlsl", "VS", "vs_5_0");
    GetMaterial()->CreatePS(L"Shader/Triangle.hlsl", "PS", "ps_5_0");
}

void Triangle::CreateInputLayout()
{
	GetMesh()->CreateInputLayout(GetMaterial());
}
