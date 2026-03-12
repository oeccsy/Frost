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
	points.push_back(base_point);

	this->parent = parent;

	guide_circle.center = base_point;
	guide_circle.radius = (HasParent()) ? Frost::SUB_BRANCH_GROW_STEP : Frost::MAIN_BRANCH_GROW_STEP;

	guide_circle.x_axis = normal;
	guide_circle.y_axis = dir;
	guide_circle.normal = guide_circle.x_axis.Cross(guide_circle.y_axis);

	guide_circle.x_axis.Normalize();
	guide_circle.y_axis.Normalize();
	guide_circle.normal.Normalize();

	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

FrostBranch::~FrostBranch() {}

void FrostBranch::Awake()
{
	Object::Awake();
	AddComponent(make_shared<Renderer>());
}

void FrostBranch::Grow(shared_ptr<MeshCollider> guide_mesh_collider)
{
	if (points.size() >= 2)
	{
		Vector3 prev_point = *(points.end() - 2);
		Vector3 cur_point = *(points.end() - 1);

		Vector3 dir = cur_point - prev_point;
		Vector3 normal = guide_circle.normal.Cross(dir);

		guide_circle.center = cur_point;
		guide_circle.x_axis = normal;
		guide_circle.y_axis = dir;

		guide_circle.x_axis.Normalize();
		guide_circle.y_axis.Normalize();
	}
	
	float theta;

	if (guide_mesh_collider->Intersects(guide_circle, theta))
	{
		Vector3 hit_point = guide_circle.center + guide_circle.radius * (cos(theta) * guide_circle.x_axis + sin(theta) * guide_circle.y_axis);
		points.push_back(hit_point);
		
		Vertex new_vertex({ hit_point, Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1)});
		
		mesh->GetVerticesRef().push_back(new_vertex);
		mesh->GetIndicesRef().push_back(points.size() - 1);
		mesh->UpdateBuffers();
	}
}

bool FrostBranch::Fork(shared_ptr<MeshCollider> guide_mesh_collider)
{
	if (points.size() <= 2) return false;

	Vector3 prev_point = *(points.end() - 2);
	Vector3 dir = guide_circle.y_axis;
	Vector3 normal = guide_circle.x_axis;

	constexpr float left_angle = ::XMConvertToRadians(-60.0f);
	constexpr float right_angle = ::XMConvertToRadians(60.0f);

	Matrix left_rot = Matrix::CreateFromAxisAngle(normal, left_angle);
	Matrix right_rot = Matrix::CreateFromAxisAngle(normal, right_angle);

	Vector3 leftDir = Vector3::Transform(dir, left_rot);
	Vector3 rightDir = Vector3::Transform(dir, right_rot);

	shared_ptr<FrostBranch> left_branch = make_shared<FrostBranch>(prev_point, leftDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));
	shared_ptr<FrostBranch> right_branch = make_shared<FrostBranch>(prev_point, rightDir, normal, static_pointer_cast<FrostBranch>(shared_from_this()));
	left_branch->Awake();
	right_branch->Awake();
	Engine::Get().GetScene()->AddObject(left_branch);
	Engine::Get().GetScene()->AddObject(right_branch);
	
	children.push_back(left_branch);
	children.push_back(right_branch);

	left_branch->Grow(guide_mesh_collider);
	right_branch->Grow(guide_mesh_collider);

	return true;
}

void FrostBranch::CreateGeometry()
{
	mesh = make_shared<DynamicMesh>();

	vector<Vertex> vertices;
	vector<uint32> indices;

	for (const Vector3& point : points)
	{
		vertices.push_back({ point, Vector3(0, 0, 0), Vector2(0, 0), Vector4(1, 1, 1, 1) });
		indices.push_back(vertices.size() - 1);
	}
	
	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mesh->CreateBuffers();
}

void FrostBranch::CreateShader()
{
	material = Material::GetMaterial<FrostBranch>();
	if (material == nullptr)
	{
		material = Material::CreateMaterial<FrostBranch>();
		material->CreateVS(L"Shader/Frost.hlsl", "VS", "vs_5_0");
		material->CreatePS(L"Shader/Frost.hlsl", "PS", "ps_5_0");
	}
}

void FrostBranch::CreateInputLayout()
{
	mesh->CreateInputLayout(material);
}
