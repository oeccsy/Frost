#include "pch.h"
#include "VectorField.h"
#include "FrostVectorField.h"
#include "Object.h"
#include "PointCloud.h"

FrostVectorField::FrostVectorField(shared_ptr<PointCloud> pointCloud)
{
	_refPoints = pointCloud;
}

FrostVectorField::~FrostVectorField() {}

Vec3 FrostVectorField::GetVector(Vec3 pos)
{
	if (!_refPoints.lock()) throw;

	Vec3 nearPointPos = _refPoints.lock().get()->GetNearPoint(pos);
	
	Vector posVector = ::XMLoadFloat3(&pos);
	Vector nearPointPosVector = ::XMLoadFloat3(&nearPointPos);
	Vector dirVector = ::XMVector3Normalize(::XMVectorSubtract(posVector, nearPointPosVector));
	
	Vec3 dir;
	::XMStoreFloat3(&dir, dirVector);

	return dir;
}
