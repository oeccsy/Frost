#include "pch.h"
#include "Component.h"
#include "Collider.h"
#include "MeshCollider.h"
#include "Object.h"
#include "PointCloud.h"
#include "PointOctree.h"
#include "Frost.h"
#include "FrostRoot.h"
#include "FrostBranch.h"
#include <random>

FrostRoot::FrostRoot(Vector3 basePoint, Vector3 normal)
{
	_basePoint = basePoint;
	_normal = normal;
}

FrostRoot::~FrostRoot() {}

void FrostRoot::Grow(shared_ptr<MeshCollider> target)
{
	for (auto branch : _growingBranches)
	{
		branch->Grow(target);
	}

	for (auto mainBranch : _growingMainBranches)
	{
		if (mainBranch->Fork(target))
		{
			vector<shared_ptr<FrostBranch>> subBranches = mainBranch->GetChildren();

			_branches.push_back(*(subBranches.end() - 1));
			_branches.push_back(*(subBranches.end() - 2));
			_growingBranches.insert(*(subBranches.end() - 1));
			_growingBranches.insert(*(subBranches.end() - 2));
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
	if (random == Vector3::Zero) random = Vector3::Forward;

	Vector3 normal = _normal;
	Vector3 biNormal = normal.Cross(random);
	biNormal.Normalize();

	int branchCount = dis(gen);

	for (int i = 0; i < branchCount; i++)
	{
		float angle = ::XMConvertToRadians(60.0f * i);
		Matrix rot = Matrix::CreateFromAxisAngle(normal, angle);
		Vector3 dir = Vector3::Transform(biNormal, rot);

		shared_ptr<FrostBranch> newBranch = make_shared<FrostBranch>(_basePoint, dir, normal, nullptr);

		_branches.push_back(newBranch);
		_growingBranches.insert(newBranch);
		_growingMainBranches.insert(newBranch);
	}
}

void FrostRoot::StopIntersectingBranches(shared_ptr<PointOctree> target)
{
	for (auto it = _growingBranches.begin(); it != _growingBranches.end();)
	{
		shared_ptr<FrostBranch> branch = *it;

		float minDist = (branch->GetParent() == nullptr) ? Frost::MAIN_BRANCH_GROW_SPEED - 0.01f : Frost::SUB_BRANCH_GROW_SPEED - 0.01f;
		BoundingSphere checkBounds({ branch->GetBranchEndPos(), minDist });

		if (target->IntersectsWithPoints(checkBounds))
		{
			if ((*it)->GetParent() == nullptr) _growingMainBranches.erase(*it);
			it = _growingBranches.erase(it);
		}
		else
		{
			it++;
		}
	}
}

vector<Vector3> FrostRoot::GetLatestEndPoints()
{
	vector<Vector3> latestEndPoints;

	for (auto& branch : _growingBranches)
	{
		latestEndPoints.push_back(branch->GetBranchEndPos());
	}

	return latestEndPoints;
}
