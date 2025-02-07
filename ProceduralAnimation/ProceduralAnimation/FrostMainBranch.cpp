#include "pch.h"
#include "Object.h"
#include "FrostMainBranch.h"
#include "Mesh.h"
#include "Material.h"

const float FrostMainBranch::GROW_SPEED = 0.1f;

FrostMainBranch::FrostMainBranch(Vertex& basePoint, Vec3& dir)
{
	Vector basePosVector = ::XMLoadFloat3(&basePoint.position);
	Vector dirVector = ::XMLoadFloat3(&dir);
	Vector endPosVector = ::XMVectorAdd(basePosVector, dirVector);
	
	Vec3 endPos;
	::XMStoreFloat3(&endPos, endPosVector);

	_branch.push_back(basePoint);
	_branch.push_back({ endPos, Vec3(0, 0, 0), Vec2(0, 0), Color(1, 1, 1, 1) });
	
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

void FrostMainBranch::GrowTo(Vec3 dir)
{
	Vector endPosVec = ::XMLoadFloat3(&_branch.back().position);
	Vector dirVec = ::XMLoadFloat3(&dir);
	Vector newPosVec = ::XMVectorAdd(endPosVec, dirVec * GROW_SPEED);

	Vertex newVertex;
	::XMStoreFloat3(&newVertex.position, newPosVec);

	_branch.push_back(newVertex);
	_mesh->SetVertices(_branch);
	_mesh->GetIndices().push_back(_branch.size() - 1);

	_mesh->UpdateBuffers();
}

