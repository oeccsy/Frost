#include "pch.h"
#include "Object.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Material.h"

Sphere::Sphere()
{
	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

Sphere::~Sphere() { }

void Sphere::Update() {}

void Sphere::CreateGeometry()
{
	float radius = 3.f;
	uint32 stackCount = 20;
	uint32 sliceCount = 20;
	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;
	float du = 1.f / static_cast<float>(sliceCount);
	float dv = 1.f / static_cast<float>(stackCount);

	vector<Vertex> vertices;

	vertices.push_back(Vertex{ Vec3(0.0f, radius, 0.0f), Vec2(0.5f, 0.0f), Color(1.f, 0.f, 0.f, 1.f) });

	for (uint32 y = 1; y <= stackCount - 1; y++)
	{
		float thetaY = y * stackAngle;

		for (uint32 x = 0; x <= sliceCount; x++)
		{
			float thetaX = x * sliceAngle;

			Vertex vertex;
			vertex.position.x = radius * sinf(thetaY) * cosf(thetaX);
			vertex.position.y = radius * cosf(thetaY);
			vertex.position.z = radius * sinf(thetaY) * sinf(thetaX);

			vertex.uv = Vec2(du * x, dv * y);

			vertex.color = Color(1.f - (float)y / stackCount, (float)y / stackCount, 0.f, 1.f);

			vertices.push_back(vertex);
		}
	}

	vertices.push_back(Vertex{ Vec3(0.0f, -radius, 0.0f), Vec2(0.5f, 1.0f), Color(0.f, 1.f, 0.f, 1.f) });


	vector<uint32> indices;

	for (uint32 i = 0; i <= sliceCount; i++)
	{
		//  [0]
		//   |  
		//  [i+1]-[i+2]
		indices.push_back(0);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
	}

	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; y++)
	{
		for (uint32 x = 0; x < sliceCount; x++)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			indices.push_back(1 + (y) * ringVertexCount + (x));
			indices.push_back(1 + (y) * ringVertexCount + (x + 1));
			indices.push_back(1 + (y + 1) * ringVertexCount + (x));

			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			indices.push_back(1 + (y + 1) * ringVertexCount + (x));
			indices.push_back(1 + (y) * ringVertexCount + (x + 1));
			indices.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	uint32 bottomIndex = static_cast<uint32>(vertices.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; i++)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		indices.push_back(bottomIndex);
		indices.push_back(lastRingStartIndex + i);
		indices.push_back(lastRingStartIndex + i + 1);
	}

	_mesh->SetVertices(vertices);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sphere::CreateShader()
{
	_material->CreateVS(L"Cube.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Cube.hlsl", "PS", "ps_5_0");
}

void Sphere::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}
