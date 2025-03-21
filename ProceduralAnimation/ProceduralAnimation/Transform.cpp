#include "pch.h"
#include "Transform.h"

Transform::Transform() {}

Transform::~Transform() {}

Vector3 Transform::GetRight()
{
	Vector rightVec = ::XMVector3TransformNormal({ 1, 0, 0 }, _world);
	
	Vector3 right;
	::XMStoreFloat3(&right, rightVec);
	return right;
}

Vector3 Transform::GetUp()
{
	Vector upVec = ::XMVector3TransformNormal({ 0, 1, 0 }, _world);

	Vector3 up;
	::XMStoreFloat3(&up, upVec);
	return up;
}

Vector3 Transform::GetForward()
{
	Vector forwardVec = ::XMVector3TransformNormal({ 0, 0, 1 }, _world);

	Vector3 forward;
	::XMStoreFloat3(&forward, forwardVec);
	return forward;
}

void Transform::SetLocalPosition(const Vector3& position)
{
	_localPosition = position;

	CalculateMatrix();
	DecomposeMatrix();
}

void Transform::SetLocalRotation(const Vector3& rotation)
{
	_localRotation = rotation;

	CalculateMatrix();
	DecomposeMatrix();
}

void Transform::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;

	CalculateMatrix();
	DecomposeMatrix();
}

void Transform::SetWorldPosition(const Vector3& position)
{
	if (HasParent())
	{
		Matrix parentWorldMatrix = _parent->GetWorldMatrix(); 
		Vector localPositionVector = ::XMVector3TransformCoord(::XMLoadFloat3(&position), ::XMMatrixInverse(nullptr, parentWorldMatrix));
		
		Vector3 localPosition;
		::XMStoreFloat3(&localPosition, localPositionVector);
		
		SetLocalPosition(localPosition);
	}
	else
	{
		SetLocalPosition(position);
	}
}

void Transform::SetWorldRotation(const Vector3& rotation)
{
	if (HasParent())
	{
		Matrix parentWorldMatrix = _parent->GetWorldMatrix();
		Vector localRotationVector = ::XMVector3TransformNormal(::XMLoadFloat3(&rotation), ::XMMatrixInverse(nullptr, parentWorldMatrix));
		
		Vector3 localRotation;
		::XMStoreFloat3(&localRotation, localRotationVector);

		SetLocalRotation(localRotation);
	}
	else
	{
		SetLocalRotation(rotation);
	}
}

void Transform::SetWorldScale(const Vector3& scale)
{
	if (HasParent())
	{
		Vector3 parentScale = _parent->GetWorldScale();
		Vector3 newScale = scale;
		newScale.x /= parentScale.x;
		newScale.y /= parentScale.y;
		newScale.z /= parentScale.z;
		SetLocalScale(newScale);
	}
	else
	{
		SetLocalScale(scale);
	}
}

void Transform::CalculateMatrix()
{
	// SRT
	Matrix s = XMMatrixScalingFromVector(XMLoadFloat3(&_localScale));
	Matrix r = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_localRotation));
	Matrix t = XMMatrixTranslationFromVector(XMLoadFloat3(&_localPosition));

	_local = s * r * t;

	if (HasParent())
	{
		_world = _local * _parent->GetWorldMatrix();
	}
	else
	{
		_world = _local;
	}

	for (const shared_ptr<Transform>& child : _children)
	{
		child->CalculateMatrix();
	}
}

Vector3 ConvertToEuler(Vector4 q)
{
	Vector3 angles;

	// roll (x-axis)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - XM_PI / 2;

	// yaw (z-axis)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::DecomposeMatrix()
{
	Vector worldPositionVector, worldRotationVector, worldScaleVector;
	::XMMatrixDecompose(&worldPositionVector, &worldRotationVector, &worldScaleVector, _world);

	::XMStoreFloat3(&_worldPosition, worldPositionVector);

	Vector4 quaternion;
	::XMStoreFloat4(&quaternion, worldRotationVector);
	_worldRotation = ConvertToEuler(quaternion);
	
	::XMStoreFloat3(&_worldPosition, worldScaleVector);
}