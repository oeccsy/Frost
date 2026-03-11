#include "pch.h"
#include "Frost.h"
#include "Object/Object.h"
#include "Component/Collider/MeshCollider.h"
#include "Types.h"
#include "SpatialPartitioning/PointCloud.h"
#include "Object/FrostRoot.h"
#include "Object/FrostBranch.h"
#include "Data/Vertex.h"
#include "Utils/MeshSampler.h"
#include <random>

const float Frost::MAIN_BRANCH_GROW_SPEED = 0.2f;
const float Frost::SUB_BRANCH_GROW_SPEED = 0.04f;
const float Frost::ROOT_FORK_DIST = 0.5f;
const float Frost::MIN_POINT_DIST = 0.f;

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
			
			frost_points->Insert(root->GetBasePoint()); // TODO
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
	for (const auto& root : forked_frost_roots)
	{
		for (const auto& branch : root->GetGrowingBranches())
		{
			frost_points->Insert(branch->GetEndPoint());
		}
		
		for (const auto& branch : root->GetNewBranches())
		{
			frost_points->Insert(branch->GetEndPoint());
		}
	}
}

void Frost::UpdateGrowingBranches()
{
	for (auto root : forked_frost_roots)
	{
		root->UpdateGrowingBranches();
	}
}