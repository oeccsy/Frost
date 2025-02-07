#include "pch.h"
#include "Object.h"
#include "PointCloud.h"
#include "Mesh.h"
#include "Material.h"
#include <random>
#include <limits>

PointCloud::PointCloud(shared_ptr<Mesh> mesh, PointType pointType, int pointCount)
{
	_baseMesh = mesh;
	_pointType = pointType;
	_pointCount = pointCount;

	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

PointCloud::~PointCloud() {}

void PointCloud::Update() {}

vector<Vertex>& PointCloud::GetPoints()
{
	return _points;
}

Vec3 PointCloud::GetNearPoint(Vec3 pos)
{
	float minDistSq = FLT_MAX;
	Vec3 nearPointPos;

	for (auto& point : _points)
	{
		Vector pointPosVec = ::XMLoadFloat3(&point.position);
		Vector originPosVec = ::XMLoadFloat3(&pos);

		Vector diff = ::XMVectorSubtract(pointPosVec, originPosVec);
		float distSq = ::XMVectorGetX(::XMVector3LengthSq(diff));
		
		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			nearPointPos = point.position;
		}
	}

	return nearPointPos;
}

void PointCloud::CreateGeometry()
{
	switch (_pointType)
	{
	case PointType::MeshVertices:
		CreateMeshVertices();
		break;
	case PointType::TriangleCenter:
		CreateTriangleCenterPoints();
		break;
	case PointType::Scatter:
		CreateScatterPoints();
		break;
	}
}

void PointCloud::CreateMeshVertices()
{
	vector<Vertex> baseVertices = _baseMesh->GetVertices();
	vector<Vertex> shuffledVertices = baseVertices;

	vector<Vertex> ownVertices;
	vector<uint32> ownIndices;

	random_device rd;
	mt19937 gen(rd());
	shuffle(shuffledVertices.begin(), shuffledVertices.end(), gen);

	_pointCount = min(_pointCount, baseVertices.size());

	for (int i = 0; i < _pointCount; i++)
	{
		ownVertices.push_back(shuffledVertices[i]);
		ownIndices.push_back(i);
	}

	_points = ownVertices;

	_mesh->SetVertices(ownVertices);
	_mesh->SetIndices(ownIndices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void PointCloud::CreateTriangleCenterPoints()
{
	if (_baseMesh->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex> baseVertices = _baseMesh->GetVertices();
	vector<uint32> baseIndices = _baseMesh->GetIndices();
	vector<Vertex> ownVertices;
	vector<uint32> ownIndices;

	vector<int> triangleFirstIndices;

	for (int i = 0; i < baseIndices.size() / 3; i++)
	{
		triangleFirstIndices.push_back(i * 3);
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(triangleFirstIndices.begin(), triangleFirstIndices.end(), gen);

	_pointCount = min(_pointCount, triangleFirstIndices.size());

	for (int i = 0; i < _pointCount; i++)
	{
		int targetIndex = triangleFirstIndices[i];
		int a_index = baseIndices[targetIndex];
		int b_index = baseIndices[targetIndex + 1];
		int c_index = baseIndices[targetIndex + 2];

		Vec3 a = baseVertices[a_index].position;
		Vec3 b = baseVertices[b_index].position;
		Vec3 c = baseVertices[c_index].position;
		Vec3 p = GetCenterPos(a, b, c);

		Vector a_normal_vec = ::XMLoadFloat3(&baseVertices[a_index].normal);
		Vector b_normal_vec = ::XMLoadFloat3(&baseVertices[b_index].normal);
		Vector c_normal_vec = ::XMLoadFloat3(&baseVertices[c_index].normal);
		Vector p_normal_vec = ::XMVector3Normalize(a_normal_vec + b_normal_vec + c_normal_vec);

		Vec3 p_normal;
		::XMStoreFloat3(&p_normal, p_normal_vec);

		ownVertices.push_back(Vertex{ p, p_normal, Vec2(0, 0), Color(1.f, 0.f, 0.f, 1.f) });
		ownIndices.push_back(i);
	}

	_points = ownVertices;

	_mesh->SetVertices(ownVertices);
	_mesh->SetIndices(ownIndices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void PointCloud::CreateScatterPoints()
{
	if (_baseMesh->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex> baseVertices = _baseMesh->GetVertices();
	vector<uint32> baseIndices = _baseMesh->GetIndices();
	vector<Vertex> ownVertices;
	vector<uint32> ownIndices;

	vector<int> triangleFirstIndices;

	for (int i = 0; i < baseIndices.size() / 3; i++)
	{
		triangleFirstIndices.push_back(i * 3);
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(triangleFirstIndices.begin(), triangleFirstIndices.end(), gen);

	for (int i = 0; i < _pointCount; i++)
	{
		int targetIndex = triangleFirstIndices[i];
		int a_index = baseIndices[targetIndex];
		int b_index = baseIndices[targetIndex + 1];
		int c_index = baseIndices[targetIndex + 2];

		Vec3 a = baseVertices[a_index].position;
		Vec3 b = baseVertices[b_index].position;
		Vec3 c = baseVertices[c_index].position;
		Vec3 p = GetRandomPosInTriangle(a, b, c);

		Vector a_normal_vec = ::XMLoadFloat3(&baseVertices[a_index].normal);
		Vector b_normal_vec = ::XMLoadFloat3(&baseVertices[b_index].normal);
		Vector c_normal_vec = ::XMLoadFloat3(&baseVertices[c_index].normal);
		Vector p_normal_vec = ::XMVector3Normalize(a_normal_vec + b_normal_vec + c_normal_vec);

		Vec3 p_normal;
		::XMStoreFloat3(&p_normal, p_normal_vec);

		ownVertices.push_back(Vertex{ p, p_normal, Vec2(0, 0), Color(1.f, 0.f, 0.f, 1.f) });
		ownIndices.push_back(i);
	}

	_points = ownVertices;

	_mesh->SetVertices(ownVertices);
	_mesh->SetIndices(ownIndices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void PointCloud::CreateShader()
{
	_material->CreateVS(L"Point.hlsl", "VS", "vs_5_0");
	_material->CreateGS(L"Point.hlsl", "GS", "gs_5_0");
	_material->CreatePS(L"Point.hlsl", "PS", "ps_5_0");
}

void PointCloud::CreateInputLayout()
{
	_mesh->CreateInputLayout(_material);
}

Vec3 PointCloud::GetCenterPos(Vec3 a, Vec3 b, Vec3 c)
{
	Vec3 p = Vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3);
	return p;
}

Vec3 PointCloud::GetRandomPosInTriangle(Vec3 a, Vec3 b, Vec3 c)
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
