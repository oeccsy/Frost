#include "pch.h"
#include "FrostRoot.h"
#include "Component/Frost.h"
#include "Object/FrostBranch.h"
#include "SpatialPartitioning//PointCloud.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include <random>

FrostRoot::FrostRoot(Vector3 base_point, Vector3 normal) : base_point(base_point), normal(normal) { }

FrostRoot::~FrostRoot() {}

void FrostRoot::Grow(shared_ptr<MeshCollider> target)
{
	for (auto branch : growing_branches)
	{
		branch->Grow(target);
	}

	for (auto main_branch : growing_main_branches)
	{
		if (main_branch->Fork(target))
		{
			vector<shared_ptr<FrostBranch>> sub_branches = main_branch->GetChildren();

			branches.push_back(*(sub_branches.end() - 1));
			branches.push_back(*(sub_branches.end() - 2));
			growing_branches.insert(*(sub_branches.end() - 1));
			growing_branches.insert(*(sub_branches.end() - 2));
		}
	}
}

void FrostRoot::Fork(shared_ptr<MeshCollider> target)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, 6);
	uniform_real_distribution<float> dis_float(-1.f, 1.f);

	Vector3 random = Vector3(dis_float(gen), dis_float(gen), dis_float(gen));
	if (random == Vector3(0, 0, 0)) random = Vector3(0, 0, -1);

	Vector3 fork_normal = normal;
	Vector3 bi_normal = fork_normal.Cross(random);
	bi_normal.Normalize();

	int branch_count = dis(gen);

	for (int i = 0; i < branch_count; i++)
	{
		float angle = ::XMConvertToRadians(60.0f * i);
		Matrix rot = Matrix::CreateFromAxisAngle(fork_normal, angle);
		Vector3 dir = Vector3::Transform(bi_normal, rot);

		shared_ptr<FrostBranch> new_branch = make_shared<FrostBranch>(base_point, dir, fork_normal, nullptr);
		new_branch->Awake();
		Engine::Get().GetScene()->AddObject(new_branch);
		
		branches.push_back(new_branch);
		growing_branches.insert(new_branch);
		growing_main_branches.insert(new_branch);
	}
}

void FrostRoot::StopIntersectingBranches(shared_ptr<PointCloud> target)
{
	for (auto it = growing_branches.begin(); it != growing_branches.end();)
	{
		shared_ptr<FrostBranch> branch = *it;

		float min_dist = (branch->GetParent() == nullptr) ? Frost::MAIN_BRANCH_GROW_SPEED - 0.01f : Frost::SUB_BRANCH_GROW_SPEED - 0.01f;
		BoundingSphere check_bounds({ branch->GetBranchEndPos(), min_dist });

		if (target->IntersectsWithPoints(check_bounds))
		{
			if ((*it)->GetParent() == nullptr) growing_main_branches.erase(*it);
			it = growing_branches.erase(it);
		}
		else
		{
			it++;
		}
	}
}

vector<Vector3> FrostRoot::GetLatestEndPoints()
{
	vector<Vector3> latest_end_points;

	for (auto& branch : growing_branches)
	{
		latest_end_points.push_back(branch->GetBranchEndPos());
	}

	return latest_end_points;
}
