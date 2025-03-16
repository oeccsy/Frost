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
#include <string>

FrostBranch::FrostBranch(Vector3& basePoint, Vector3& dir, Vector3& normal, shared_ptr<FrostBranch> parent)
{
	_parent = parent;

	_mesh = make_shared<DynamicMesh>();

	Vector3 endPos = basePoint + dir * Frost::MAIN_BRANCH_GROW_SPEED;
	_branch.push_back({ basePoint, normal, Vector2(0, 0), Color(1, 1, 1, 1) });
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
	_guideCircle.radius = Frost::MAIN_BRANCH_GROW_SPEED;

	_guideCircle.xAxis = normal;
	_guideCircle.yAxis = dir;
	_guideCircle.xAxis.Normalize();
	_guideCircle.yAxis.Normalize();
	
	_guideCircle.normal = _guideCircle.xAxis.Cross(_guideCircle.yAxis);
}

FrostBranch::~FrostBranch() {}

void FrostBranch::Grow(shared_ptr<MeshCollider> target)
{
	int size = _branch.size();
	Vector3 prevPos = _branch[size - 2].position;
	Vector3 curPos = _branch[size - 1].position;
	
	Vector3 dir = curPos - prevPos;
	Vector3 normal = dir.Cross(_guideCircle.normal);
	
	_guideCircle.center = curPos;
	_guideCircle.xAxis = normal;
	_guideCircle.yAxis = dir;
	_guideCircle.radius = (_parent == nullptr) ? Frost::MAIN_BRANCH_GROW_SPEED : Frost::SUB_BRANCH_GROW_SPEED;

	float theta;
	
	if (target->Intersects(_guideCircle, theta))
	{
		Vector3 hitPoint = _guideCircle.center + _guideCircle.radius * (cos(theta) * _guideCircle.xAxis + sin(theta) * _guideCircle.yAxis);
		
		Vertex newVertex({ hitPoint, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1)});

		_branch.push_back(newVertex);
		_mesh->GetVertices().push_back(newVertex);
		_mesh->GetIndices().push_back(_branch.size() - 1);
		_mesh->UpdateBuffers();
	}
}

void FrostBranch::Fork()
{
	int size = _branch.size();
	Vector3 prevPos = _branch[size - 2].position;
	Vector3 curPos = _branch[size - 1].position;

	Vector3 dir = curPos - prevPos;
	Vector3 normal = dir.Cross(_guideCircle.normal);

	constexpr float leftAngle = ::XMConvertToRadians(-60.0f);
	constexpr float rightAngle = ::XMConvertToRadians(60.0f);

	Matrix leftRot = Matrix::CreateFromAxisAngle(normal, leftAngle);
	Matrix rightRot = Matrix::CreateFromAxisAngle(normal, rightAngle);

	Vector3 leftDir = Vector3::Transform(dir, leftRot);
	Vector3 rightDir = Vector3::Transform(dir, rightRot);

	shared_ptr<FrostBranch> leftBranch = make_shared<FrostBranch>(prevPos, leftDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));
	shared_ptr<FrostBranch> rightBranch = make_shared<FrostBranch>(prevPos, rightDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));

	_children.push_back(leftBranch);
	_children.push_back(rightBranch);
}