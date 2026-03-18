#include "pch.h"
#include "Frost.h"
#include "Object/Object.h"
#include "Component/Collider/MeshCollider.h"
#include "Utils/MeshSampler.h"
#include "Data/Vertex.h"
#include "Types.h"
#include "SpatialPartitioning/PointCloud.h"
#include "Engine.h"
#include "Object/FrostRoot.h"
#include "Object/FrostBranch.h"
#include "Object/FrostVisual.h"
#include "Render/Mesh.h"
#include "Render/Renderer/Renderer.h"
#include <random>

const float Frost::MAIN_BRANCH_GROW_STEP = 0.20f;
const float Frost::SUB_BRANCH_GROW_STEP = 0.04f;
const float Frost::MAIN_BRANCH_STOP_DIST = 0.19f;
const float Frost::SUB_BRANCH_STOP_DIST = 0.03f;
const float Frost::ROOT_FORK_DIST = 0.5f;
const int Frost::MAX_BRANCHES_PER_FORK = 6;

Frost::Frost() {}

Frost::~Frost() {}

void Frost::Awake()
{
	guide_mesh_collider = GetOwner()->GetComponent<MeshCollider>();
	frost_points = make_shared<PointCloud>(BoundingBox(Vector3(0, 0, 0), Vector3(6, 6, 6)));
	
	vector<Vertex> base_points = MeshSampler::GenerateScatterPoints(GetOwner()->GetMesh(), 50);
	for (auto& base_point : base_points)
	{
		unforked_frost_roots.push_back(make_shared<FrostRoot>(base_point.position, base_point.normal));
	}
	
	frost_visual = Engine::Get().GetScene()->SpawnObject<FrostVisual>();
	frost_visual->AddComponent(make_shared<Renderer>());
}

void Frost::Update()
{
	GrowBranches();
	ForkBranches();
	ForkCloseRoots();
	StopCloseBranches();
	UpdateFrostPoints();
	UpdateGrowingBranches();
}

void Frost::ForkRandomRoots()
{
	float threshold = 0.1f;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	
	for (int i=0; i<unforked_frost_roots.size();)
	{
		auto root = unforked_frost_roots[i];
		
		if (dis(gen) < threshold)
		{
			root->Fork(guide_mesh_collider);
			
			unforked_frost_roots[i] = unforked_frost_roots.back();
			unforked_frost_roots.pop_back();
			forked_frost_roots.push_back(root);
			
			frost_points->Insert(root->GetBasePoint());
		}
		else
		{
			++i;
		}
	}
}

void Frost::GrowBranches()
{
	for (const auto& root : forked_frost_roots)
	{
		root->GrowBranches(guide_mesh_collider);
	}
}

void Frost::ForkBranches()
{
	for (const auto& root : forked_frost_roots)
	{
		root->ForkBranches(guide_mesh_collider);
	}
}

void Frost::ForkCloseRoots()
{
	for (int i=0; i<unforked_frost_roots.size();)
	{
		auto root = unforked_frost_roots[i];
		
		BoundingSphere check_bounds({ root->GetBasePoint(), ROOT_FORK_DIST });

		if (frost_points->IntersectsWithPoints(check_bounds))
		{
			root->Fork(guide_mesh_collider);
			
			unforked_frost_roots[i] = unforked_frost_roots.back();
			unforked_frost_roots.pop_back();
			forked_frost_roots.push_back(root);
			
			frost_points->Insert(root->GetBasePoint());
		}
		else
		{
			++i;
		}
	}
}

void Frost::StopCloseBranches()
{
	for (const auto& root : forked_frost_roots)
	{
		root->StopCloseBranches(frost_points);
	}
}

void Frost::UpdateFrostPoints()
{
	vector<Vertex>& vertices = frost_visual->GetMesh()->GetVerticesRef();
	vector<uint32>& indices = frost_visual->GetMesh()->GetIndicesRef();
	
	for (const auto& root : forked_frost_roots)
	{
		for (const auto& branch : root->GetGrowingBranches())
		{
			frost_points->Insert(branch->GetEndPoint());
			
			vertices.push_back( { branch->GetPrevEndPoint(), Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1) });
			vertices.push_back( { branch->GetEndPoint(), Vector3(0, 0, 0), Vector2(0, 0), Color(1, 1, 1, 1) });
			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 1);
		}
		
		for (const auto& branch : root->GetNewBranches())
		{
			frost_points->Insert(branch->GetEndPoint());
		}
	}
	
	frost_visual->GetMesh()->UpdateBuffers();
}

void Frost::UpdateGrowingBranches()
{
	for (auto root : forked_frost_roots)
	{
		root->UpdateGrowingBranches();
	}
}