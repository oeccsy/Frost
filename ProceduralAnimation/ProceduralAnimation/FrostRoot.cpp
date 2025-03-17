#include "pch.h"
#include "Object.h"
#include "FrostBranch.h"
#include "Component.h"
#include "Collider.h"
#include "MeshCollider.h"
#include "PointOctree.h"
#include "FrostRoot.h"
#include "Frost.h"
#include <random>

FrostRoot::FrostRoot(Vector3 basePoint, Vector3 normal)
{
	_basePoint = basePoint;
	_normal = normal;
}

FrostRoot::~FrostRoot() {}

void FrostRoot::ForkRoot(shared_ptr<MeshCollider> target)
{
	Vector3 temp = Vector3(1, 0, 0);
	Vector3 normal = _normal;
	Vector3 biNormal = normal.Cross(temp);
	biNormal.Normalize();

	/*random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, MAX_BRANCH_COUNT);

	int branchCount = dis(gen);*/

	int branchCount = 1;

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

void FrostRoot::GrowBranches(shared_ptr<MeshCollider> target)
{
	for (auto branch : _growingBranches)
	{
		branch->Grow(target);
	}
}

void FrostRoot::ForkMainBranches(shared_ptr<MeshCollider> target)
{
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

void FrostRoot::DisableGrowth(shared_ptr<PointOctree> target)
{
	for (auto it = _growingBranches.begin(); it != _growingBranches.end();)
	{
		shared_ptr<FrostBranch> branch = *it;

		BoundingSphere checkBounds({ branch->GetBranchEndPos(), Frost::MIN_POINT_DIST });

		if (target->IntersectsWithPoints(checkBounds))
		{
			OutputDebugStringA("Remove Branch");
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
