#include "pch.h"
#include "Object.h"
#include "FrostMainBranch.h"
#include "Mesh.h"
#include "Material.h"
#include "DynamicMesh.h"
#include "FrostRoot.h"
#include "Frost.h"
#include "Component.h"
#include "Collider.h"
#include "MeshCollider.h"
#include "Sphere.h"

const float FrostMainBranch::GROW_SPEED = 1.f;

FrostMainBranch::FrostMainBranch(Vertex& basePoint, Vector3& dir, Vector3& normal, FrostRoot& parent) : _parent(parent)
{
	_mesh = make_shared<DynamicMesh>();

	Vector3 endPos = basePoint.position + dir * GROW_SPEED;
	_branch.push_back(basePoint);
	_branch.push_back({ endPos, normal, Vector2(0, 0), Color(1, 1, 1, 1) });
	
	vector<uint32> indices = { 0, 1 };

	_mesh->SetVertices(_branch);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	_material->CreateVS(L"Cube.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Cube.hlsl", "PS", "ps_5_0");

	_mesh->CreateInputLayout(_material);

	_guideCircle.center = endPos;
	_guideCircle.radius = GROW_SPEED;

	_guideCircle.xAxis = normal;
	_guideCircle.yAxis = dir;
	_guideCircle.xAxis.Normalize();
	_guideCircle.yAxis.Normalize();
	
	_guideCircle.normal = _guideCircle.xAxis.Cross(_guideCircle.yAxis);
}

FrostMainBranch::~FrostMainBranch() {}

void FrostMainBranch::Grow()
{
	int size = _branch.size();
	Vector3 prevPos = _branch[size - 2].position;
	Vector3 curPos = _branch[size - 1].position;
	
	Vector3 dir = curPos - prevPos;
	Vector3 normal = dir.Cross(_guideCircle.normal);
	
	_guideCircle.center = curPos;
	_guideCircle.xAxis = normal;
	_guideCircle.yAxis = dir;

	float theta;
	shared_ptr<MeshCollider> meshCollider = GetParent().GetParent().GetSphere()->GetComponent<MeshCollider>();
	
	if (meshCollider->Intersects(_guideCircle, theta))
	{
		Vector3 hitPoint = _guideCircle.center + _guideCircle.radius * (cos(theta) * _guideCircle.xAxis + sin(theta) * _guideCircle.yAxis);
		
		Vertex newVertex({ hitPoint, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1)});

		_branch.push_back(newVertex);
		_mesh->GetVertices().push_back(newVertex);
		_mesh->GetIndices().push_back(_branch.size() - 1);
		_mesh->UpdateBuffers();
	}

	// TODO : 성장 중단
}

