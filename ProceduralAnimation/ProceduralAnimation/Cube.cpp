#include "pch.h"
#include "Object.h"
#include "Mesh.h"
#include "Material.h"
#include "Cube.h"
#include "Transform.h"


Cube::Cube()
{
	CreateGeometry();
	CreateShader();
	CreateInputLayout();
	_transform->SetLocalPosition(Vec3(0, 0, 30));
	_transform->SetLocalRotation(Vec3(0.5f, 0.5f, 0.f));
}

Cube::~Cube() {}

void Cube::Update() { }

void Cube::CreateGeometry()
{
	vector<Vertex> vertices;
	vertices.resize(8);

	vertices[0].position = Vec3(-0.5f, -0.5f, -0.5f);
	vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[1].position = Vec3(-0.5f, 0.5f, -0.5f);
	vertices[1].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[2].position = Vec3(0.5f, 0.5f, -0.5f);
	vertices[2].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[3].position = Vec3(0.5f, -0.5f, -0.5f);
	vertices[3].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[4].position = Vec3(-0.5f, -0.5f, 0.5f);
	vertices[4].color = Color(1.f, 1.f, 1.f, 1.f);
	vertices[5].position = Vec3(-0.5f, 0.5f, 0.5f);
	vertices[5].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[6].position = Vec3(0.5f, 0.5f, 0.5f);
	vertices[6].color = Color(0.f, 1.f, 0.f, 1.f);
	vertices[7].position = Vec3(0.5f, -0.5f, 0.5f);
	vertices[7].color = Color(0.f, 0.f, 1.f, 1.f);

	vector<uint32> indices;
	indices.assign({
		// Front face
		0, 1, 2, 0, 2, 3,
		// Back face
		4, 6, 5, 4, 7, 6,
		// Left face
		4, 5, 1, 4, 1, 0,
		// Right face
		3, 2, 6, 3, 6, 7,
		// Top face
		1, 5, 6, 1, 6, 2,
		// Bottom face
		4, 0, 3, 4, 3, 7,
	});

	_mesh->SetVertices(vertices);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Cube::CreateShader()
{
	_material->CreateVS(L"Cube.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Cube.hlsl", "PS", "ps_5_0");
}

void Cube::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}