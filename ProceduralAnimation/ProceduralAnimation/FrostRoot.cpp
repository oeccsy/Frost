#include "pch.h"
#include "Object.h"
#include "FrostMainBranch.h"
#include "FrostRoot.h"
#include "Frost.h"
#include <random>

FrostRoot::FrostRoot(Frost& parent, Vertex& basePoint) : _parent(parent), _basePoint(basePoint)
{
	_isForked = false;
}

FrostRoot::~FrostRoot() {}

void FrostRoot::Fork()
{
	Vector3 temp = Vector3(1, 0, 0);
	Vector3 normal = _basePoint.normal;
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

		shared_ptr<FrostMainBranch> newBranch = make_shared<FrostMainBranch>(_basePoint, dir, normal, *this);
		_branches.push_back(newBranch);
		_growingBranches.insert(newBranch);
	}
}