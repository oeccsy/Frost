#include "pch.h"
#include "Object.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Material.h"


Triangle::Triangle()
{
	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

Triangle::~Triangle() {}

void Triangle::Update() {}

void Triangle::CreateGeometry()
{
	vector<Vertex> vertices;
	vertices.resize(3);

	vertices[0].position = Vec3(0.5f, 0.5f, 0);
	vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[1].position = Vec3(-0.5f, 0.5f, 0);
	vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
	vertices[2].position = Vec3(-0.5f, -0.5f, 0);
	vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);

	vector<uint32> indices;
	indices = { 0, 2, 1};

	_mesh->SetVertices(vertices);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Triangle::CreateShader()
{
	_material->CreateVS(L"Point.hlsl", "VS", "vs_5_0");
	_material->CreateGS(L"Point.hlsl", "GS", "gs_5_0");
	_material->CreatePS(L"Point.hlsl", "PS", "ps_5_0");
}

void Triangle::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}