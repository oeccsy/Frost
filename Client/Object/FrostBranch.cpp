#include "pch.h"
#include "FrostBranch.h"
#include "Component/Frost.h"
#include "Render/Mesh.h"
#include "Render/DynamicMesh.h"
#include "Render/Material.h"
#include "Component/Collider/MeshCollider.h"
#include "Render/Renderer/Renderer.h"
#include "Engine.h"
#include "Scene/Scene.h"

FrostBranch::FrostBranch(Vector3& base_point, Vector3& dir, Vector3& normal, shared_ptr<FrostBranch> parent)
{
	this->parent = parent;
	guide_circle.center = base_point;
	guide_circle.radius = (parent == nullptr) ? Frost::MAIN_BRANCH_GROW_SPEED : Frost::SUB_BRANCH_GROW_SPEED;

	guide_circle.xAxis = normal;
	guide_circle.yAxis = dir;
	guide_circle.normal = guide_circle.xAxis.Cross(guide_circle.yAxis);

	guide_circle.xAxis.Normalize();
	guide_circle.yAxis.Normalize();
	guide_circle.normal.Normalize();

	mesh = make_shared<DynamicMesh>();

	branch.push_back({ base_point, normal, Vector2(0, 0), Color(1, 1, 1, 1) });
	vector<uint32> indices = { 0 };

	mesh->SetVertices(branch);
	mesh->SetIndices(indices);

	mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mesh->CreateBuffers();
	
	material = Material::GetMaterial<FrostBranch>();
	if (material == nullptr)
	{
		material = Material::CreateMaterial<FrostBranch>();
		material->CreateVS(L"Shader/Frost.hlsl", "VS", "vs_5_0");
		material->CreatePS(L"Shader/Frost.hlsl", "PS", "ps_5_0");
	}
	
	mesh->CreateInputLayout(material);
}

FrostBranch::~FrostBranch() {}

void FrostBranch::Awake()
{
	Object::Awake();
	AddComponent(make_shared<Renderer>());
}

bool FrostBranch::Grow(shared_ptr<MeshCollider> target)
{
	float theta;

	if (target->Intersects(guide_circle, theta))
	{
		Vector3 hit_point = guide_circle.center + guide_circle.radius * (cos(theta) * guide_circle.xAxis + sin(theta) * guide_circle.yAxis);
		
		Vertex new_vertex({ hit_point, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1)});
		branch.push_back(new_vertex);
		
		vector<Vertex> new_vertices = mesh->GetVertices();
		new_vertices.push_back(new_vertex);
		vector<uint32> new_indices = mesh->GetIndices();
		new_indices.push_back(branch.size() - 1);
		
		mesh->SetVertices(new_vertices);
		mesh->SetIndices(new_indices);
		mesh->UpdateBuffers();

		Vector3 new_dir = (branch.end() - 1)->position - (branch.end() - 2)->position;
		Vector3 new_normal = new_dir.Cross(guide_circle.normal);

		guide_circle.center = hit_point;
		guide_circle.xAxis = new_normal;
		guide_circle.yAxis = new_dir;

		guide_circle.xAxis.Normalize();
		guide_circle.yAxis.Normalize();

		return true;
	}
	else
	{
		return false;
	}
}

bool FrostBranch::Fork(shared_ptr<MeshCollider> target)
{
	if (branch.size() < 3) return false;

	Vector3 prev_pos = (branch.end() - 2)->position;
	Vector3 dir = guide_circle.yAxis;
	Vector3 normal = guide_circle.xAxis;

	constexpr float leftAngle = ::XMConvertToRadians(-60.0f);
	constexpr float rightAngle = ::XMConvertToRadians(60.0f);

	Matrix leftRot = Matrix::CreateFromAxisAngle(normal, leftAngle);
	Matrix rightRot = Matrix::CreateFromAxisAngle(normal, rightAngle);

	Vector3 leftDir = Vector3::Transform(dir, leftRot);
	Vector3 rightDir = Vector3::Transform(dir, rightRot);

	shared_ptr<FrostBranch> leftBranch = make_shared<FrostBranch>(prev_pos, leftDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));
	shared_ptr<FrostBranch> rightBranch = make_shared<FrostBranch>(prev_pos, rightDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));
	leftBranch->Awake();
	rightBranch->Awake();
	Engine::Get().GetScene()->AddObject(leftBranch);
	Engine::Get().GetScene()->AddObject(rightBranch);
	
	children.push_back(leftBranch);
	children.push_back(rightBranch);

	leftBranch->Grow(target);
	rightBranch->Grow(target);

	return true;
}