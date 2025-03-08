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

const float FrostMainBranch::GROW_SPEED = 0.1f;

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
}

FrostMainBranch::~FrostMainBranch() {}

void FrostMainBranch::Grow()
{
	int size = _branch.size();
	Vector3 prevPos = _branch[size - 2].position;
	Vector3 curPos = _branch[size - 1].position;
	
	Vector3 normal = _branch.back().normal;
	Vector3 dir = curPos - prevPos;
	Vector3 biNormal = normal.Cross(dir);
	dir.Normalize();
	biNormal.Normalize();

	Vector3 nextStep = curPos + dir * GROW_SPEED;
	
	Ray ray({ nextStep, -normal });
	Vector3 hitPos;

	shared_ptr<MeshCollider> meshCollider = GetParent().GetParent().GetSphere()->GetComponent<MeshCollider>();
	
	if (meshCollider->Intersects(ray, hitPos))
	{
		Vector3 newDir = hitPos - curPos;
		Vector3 newNormal = dir.Cross(biNormal);

		Vertex newVertex({ hitPos, newNormal, Vector2(0, 0), Color(1, 1, 1, 1) });
		
		_branch.push_back(newVertex);
		_mesh->GetVertices().push_back(newVertex);
		_mesh->GetIndices().push_back(_branch.size() - 1);
		_mesh->UpdateBuffers();
	}
	else
	{
		// TODO : 성장 중단
	}
}

