#include "pch.h"
#include "Frost.h"
#include "Object/Object.h"
#include "Component/Collider/MeshCollider.h"
#include "Types.h"
#include "SpatialPartitioning/PointCloud.h"
#include "Object/FrostRoot.h"
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
	base_points = make_shared<PointCloud>(BoundingBox(Vector3(0, 0, 0), Vector3(10, 10, 10)));
	base_points->GenerateScatterPoints(GetOwner()->GetMesh(), 50);

	unforked_roots = make_shared<PointCloud>(BoundingBox(Vector3(0, 0, 0), Vector3(6, 6, 6)));
	frost_points = make_shared<PointCloud>(BoundingBox(Vector3(0, 0, 0), Vector3(6, 6, 6)));

	for (auto& base_point : base_points->GetAllPoints())
	{
		shared_ptr<FrostRoot> new_root = make_shared<FrostRoot>(base_point, base_point);
		unforked_frost_roots.insert(new_root);
		unforked_roots->Insert(new_root->GetBasePoint());
	}
}

void Frost::Update()
{
	Grow();
	ForkCloseRoots();
}

void Frost::Grow()
{
	for (auto root : forked_frost_roots)
	{
		root->Grow(guide_mesh_collider);

		vector<Vector3> endPoints = root->GetLatestEndPoints();
		root->StopIntersectingBranches(frost_points);

		for (Vector3& point : endPoints)
		{
			frost_points->Insert(point);
		}
	}
}

void Frost::ForkCloseRoots()
{
	for (auto it = unforked_frost_roots.begin(); it != unforked_frost_roots.end();)
	{
		auto root = *it;

		BoundingSphere checkBounds({ root->GetBasePoint(), ROOT_FORK_DIST });

		if (frost_points->IntersectsWithPoints(checkBounds))
		{
			root->Fork(guide_mesh_collider);
			it = unforked_frost_roots.erase(it);

			forked_frost_roots.insert(root);
			frost_points->Insert(root->GetBasePoint());
		}
		else
		{
			it++;
		}
	}
}

void Frost::ForkRandomRoots()
{
	float threshold = 0.1f;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	
	for (auto it = unforked_frost_roots.begin(); it != unforked_frost_roots.end();)
	{
		auto root = *it;

		if (dis(gen) < threshold)
		{
			root->Fork(guide_mesh_collider);

			it = unforked_frost_roots.erase(it);
			forked_frost_roots.insert(root);
			frost_points->Insert(root->GetBasePoint());
		}
		else
		{
			it++;
		}
	}
}