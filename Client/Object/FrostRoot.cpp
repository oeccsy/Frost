#include "pch.h"
#include "FrostRoot.h"
#include "Component/Frost.h"
#include "Object/FrostBranch.h"
#include "SpatialPartitioning/PointCloud.h"
#include <random>

FrostRoot::FrostRoot(Vector3 base_point, Vector3 normal) : base_point(base_point), normal(normal) { }

FrostRoot::~FrostRoot() {}

void FrostRoot::Fork(shared_ptr<MeshCollider> guide_mesh_collider)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, Frost::MAX_BRANCHES_PER_FORK);
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
		new_branches.push_back(new_branch);
	}
}

void FrostRoot::GrowBranches(shared_ptr<class MeshCollider> guide_mesh_collider)
{
	for (const auto& branch : growing_branches)
	{
		branch->Grow(guide_mesh_collider);
	}
}

void FrostRoot::ForkBranches(shared_ptr<class MeshCollider> guide_mesh_collider)
{
	for (const auto& branch : growing_branches)
	{
		if (branch->HasParent()) continue;
		if (branch->Fork(guide_mesh_collider))
		{
			vector<shared_ptr<FrostBranch>> sub_branches = branch->GetChildren();
			new_branches.push_back(*(sub_branches.end() - 2));
			new_branches.push_back(*(sub_branches.end() - 1));
		}
	}
}

void FrostRoot::StopCloseBranches(shared_ptr<PointCloud> frost_points)
{
	for (int i = 0; i < growing_branches.size();)
	{
		shared_ptr<FrostBranch> branch = growing_branches[i];

		float stop_dist = (branch->HasParent()) ? Frost::SUB_BRANCH_STOP_DIST : Frost::MAIN_BRANCH_STOP_DIST;
		BoundingSphere check_bounds({ branch->GetEndPoint(), stop_dist });

		if (frost_points->IntersectsWithPoints(check_bounds))
		{
			growing_branches[i] = growing_branches.back();
			growing_branches.pop_back();
		}
		else
		{
			++i;
		}
	}
}

void FrostRoot::UpdateGrowingBranches()
{
	for (auto new_branch : new_branches)
	{
		growing_branches.push_back(new_branch);
	}
	
	new_branches.clear();
}
