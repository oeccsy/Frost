#include "pch.h"
#include "Object.h"
#include "FrostMainBranch.h"
#include "FrostRoot.h"
#include <random>

FrostRoot::FrostRoot(Vertex& basePoint)
{
	_basePoint = &basePoint;
	_mainBranches.reserve(MAX_BRANCH_COUNT);
	_isForked = false;
}

FrostRoot::~FrostRoot() {}

void FrostRoot::Fork()
{
	Vec3 temp = Vec3(1, 0, 0);
	Vector normalVector = ::XMLoadFloat3(&_basePoint->normal);
	Vector tempVector = ::XMLoadFloat3(&temp);

	Vector biNormalVector = ::XMVector3Normalize(::XMVector3Cross(normalVector, tempVector)); 

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(1, MAX_BRANCH_COUNT);

	int branchCount = dis(gen);

	for (int i = 0; i < branchCount; i++)
	{
		float angle = ::XMConvertToRadians(60.0f * i);
		Matrix rot = ::XMMatrixRotationAxis(normalVector, angle);
		Vector dirVector = ::XMVector3Transform(biNormalVector, rot);
		Vector stepVector = ::XMVectorScale(dirVector, 0.1f);
		
		Vec3 step;
		::XMStoreFloat3(&step, stepVector);

		_mainBranches.emplace_back(*_basePoint, step);
	}
}

void FrostRoot::Grow()
{
	for (auto& branch : _mainBranches)
	{
		//branch.GrowTo();
	}
}
