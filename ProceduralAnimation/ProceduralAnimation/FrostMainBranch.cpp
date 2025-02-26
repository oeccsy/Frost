#include "pch.h"
#include "Object.h"
#include "FrostMainBranch.h"
#include "Mesh.h"
#include "Material.h"
#include "DynamicMesh.h"

const float FrostMainBranch::GROW_SPEED = 0.5f;

FrostMainBranch::FrostMainBranch(Vertex& basePoint, Vector3& dir)
{
	_mesh = make_shared<DynamicMesh>();

	Vector basePosVector = ::XMLoadFloat3(&basePoint.position);
	Vector dirVector = ::XMLoadFloat3(&dir);
	Vector endPosVector = ::XMVectorAdd(basePosVector, dirVector);
	
	Vector3 endPos;
	::XMStoreFloat3(&endPos, endPosVector);

	_branch.push_back(basePoint);
	_branch.push_back({ endPos, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1) });
	
	vector<uint32> indices = { 0, 1 };

	_mesh->SetVertices(_branch);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	_material->CreateVS(L"Cube.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Cube.hlsl", "PS", "ps_5_0");

	_mesh->CreateInputLayout(_material);
}

FrostMainBranch::~FrostMainBranch() {}

void FrostMainBranch::GrowTo(Vector3 dir)
{
	Vector endPosVec = ::XMLoadFloat3(&_branch.back().position);
	Vector stepVec = ::XMVectorScale(::XMLoadFloat3(&dir), GROW_SPEED);
	Vector newPosVec = ::XMVectorAdd(endPosVec, stepVec);

	Vertex newVertex;
	::XMStoreFloat3(&newVertex.position, newPosVec);
	newVertex.normal = Vector3{ 0, 0, 0 };
	newVertex.uv = Vector2{ 0, 0 };
	newVertex.color = Color{ 1, 1, 1, 1 };

	_branch.push_back(newVertex);
	_mesh->SetVertices(_branch);
	_mesh->GetIndices().push_back(_branch.size() - 1);

	_mesh->UpdateBuffers();
}

