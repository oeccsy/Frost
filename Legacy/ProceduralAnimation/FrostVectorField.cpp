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

Vector3 FrostVectorField::GetVector(Vector3 pos)
{
	if (!_refPoints.lock()) throw;

	Vector3 nearPointPos = _refPoints.lock().get()->GetNearPoint(pos);
	
	Vector posVector = ::XMLoadFloat3(&pos);
	Vector nearPointPosVector = ::XMLoadFloat3(&nearPointPos);
	Vector dirVector = ::XMVector3Normalize(::XMVectorSubtract(posVector, nearPointPosVector));
	
	Vector3 dir;
	::XMStoreFloat3(&dir, dirVector);

	// float angle = radians(anoise(pos * smoothness) * max_ang);
	// vector4 rot = quaternion(angle, @n);
	// dir = qrotate(rot, dir);
	// v@dir = normalize(dir);

	return dir;
}
