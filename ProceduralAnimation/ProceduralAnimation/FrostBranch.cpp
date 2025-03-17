#include "pch.h"
#include "Object.h"
#include "FrostBranch.h"
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

	_guideCircle.center = basePoint;
	_guideCircle.radius = (_parent == nullptr) ? Frost::MAIN_BRANCH_GROW_SPEED : Frost::SUB_BRANCH_GROW_SPEED;

	_guideCircle.xAxis = normal;
	_guideCircle.yAxis = dir;
	_guideCircle.normal = _guideCircle.xAxis.Cross(_guideCircle.yAxis);

	_guideCircle.xAxis.Normalize();
	_guideCircle.yAxis.Normalize();
	_guideCircle.normal.Normalize();

	_mesh = make_shared<DynamicMesh>();

	_branch.push_back({ basePoint, normal, Vector2(0, 0), Color(1, 1, 1, 1) });
	vector<uint32> indices = { 0 };

	_mesh->SetVertices(_branch);
	_mesh->SetIndices(indices);

	_mesh->CreateBuffers();
	_mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	_material->CreateVS(L"Cube.hlsl", "VS", "vs_5_0");
	_material->CreatePS(L"Cube.hlsl", "PS", "ps_5_0");

	_mesh->CreateInputLayout(_material);

	string endPointString = to_string(_branch.back().position.x) + " " + to_string(_branch.back().position.y) + " " + to_string(_branch.back().position.z);
	OutputDebugStringA("Create Branch \n");
	OutputDebugStringA(("Endpoint : " + endPointString + "\n").c_str());
}

FrostBranch::~FrostBranch() {}

bool FrostBranch::Grow(shared_ptr<MeshCollider> target)
{
	float theta;

	if (target->Intersects(_guideCircle, theta))
	{
		Vector3 hitPoint = _guideCircle.center + _guideCircle.radius * (cos(theta) * _guideCircle.xAxis + sin(theta) * _guideCircle.yAxis);
		
		Vertex newVertex({ hitPoint, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1)});

		_branch.push_back(newVertex);
		_mesh->GetVertices().push_back(newVertex);
		_mesh->GetIndices().push_back(_branch.size() - 1);
		_mesh->UpdateBuffers();

		Vector3 newDir = (_branch.end() - 1)->position - (_branch.end() - 2)->position;
		Vector3 newNormal = newDir.Cross(_guideCircle.normal);

		_guideCircle.center = hitPoint;
		_guideCircle.xAxis = newNormal;
		_guideCircle.yAxis = newDir;

		string endPointString = to_string(_branch.back().position.x) + " " + to_string(_branch.back().position.y) + " " + to_string(_branch.back().position.z);
		OutputDebugStringA("Hit Success \n");
		OutputDebugStringA(("Endpoint : " + endPointString + "\n").c_str());
		_guideCircle.xAxis.Normalize();
		_guideCircle.yAxis.Normalize();

		return true;
	}
	else
	{
		return false;
	}
}

bool FrostBranch::Fork(shared_ptr<MeshCollider> target)
{
	if (_branch.size() < 2) return false;

	Vector3 prevPos = (_branch.end() - 2)->position;
	Vector3 dir = _guideCircle.yAxis;
	Vector3 normal = _guideCircle.xAxis;

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

	leftBranch->Grow(target);
	rightBranch->Grow(target);

	return true;
}