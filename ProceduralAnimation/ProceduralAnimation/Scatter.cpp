#include "pch.h"
#include "Object.h"
#include "Scatter.h"
#include "Mesh.h"
#include "Material.h"
#include <random>

Scatter::Scatter(shared_ptr<Mesh> mesh)
{
	CreateGeometry(mesh);
	CreateShader();
	CreateInputLayout();
}

Scatter::~Scatter() {}

void Scatter::Update() {}

void Scatter::CreateGeometry(shared_ptr<Mesh> mesh)
{
	if (mesh->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex> inputVertices = mesh->GetVertices();
	vector<uint32> inputIndices = mesh->GetIndices();
	vector<Vertex> scatterVertices;
	vector<uint32> scatterIndices;

	int targetTriangleCount = 20;
	vector<int> targetTriangleIndices;

	for (int i = 0; i < inputIndices.size() / 3; i++)
	{
		targetTriangleIndices.push_back(i * 3);
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(targetTriangleIndices.begin(), targetTriangleIndices.end(), gen);

	for (int i = 0; i < targetTriangleCount; i++)
	{
		int targetIndex = targetTriangleIndices[i];
		int a_index = inputIndices[targetIndex];
		int b_index = inputIndices[targetIndex + 1];
		int c_index = inputIndices[targetIndex + 2];

		Vec3 a = inputVertices[a_index].position;
		Vec3 b = inputVertices[b_index].position;
		Vec3 c = inputVertices[c_index].position;

		Vec3 p = GetRandomPosInTriangle(a, b, c);
		scatterVertices.push_back(Vertex{ p, Vec3(0, 0, 0), Vec2(0, 0), Color(1.f, 0.f, 0.f, 1.f) });
		scatterIndices.push_back(i);
	}

	_mesh->SetVertices(scatterVertices);
	_mesh->SetIndices(scatterIndices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Scatter::CreateShader()
{
	_material->CreateVS(L"Point.hlsl", "VS", "vs_5_0");
	_material->CreateGS(L"Point.hlsl", "GS", "gs_5_0");
	_material->CreatePS(L"Point.hlsl", "PS", "ps_5_0");
}

void Scatter::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}

Vec3 Scatter::GetRandomPosInTriangle(Vec3 a, Vec3 b, Vec3 c)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);

	float r1 = dis(gen);
	float r2 = dis(gen);

	float sqrt_r1 = sqrt(r1);
	float m = 1 - sqrt_r1;
	float n = sqrt_r1 * r2;
	float l = sqrt_r1 * (1 - r2);

	Vec3 p = Vec3(m * a.x + n * b.x + l * c.x, m * a.y + n * b.y + l * c.y, m * a.z + n * b.z + l * c.z);
	return p;
}
