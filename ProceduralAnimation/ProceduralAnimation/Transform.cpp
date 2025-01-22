#include "pch.h"
#include "Transform.h"

Transform::Transform() {}

Transform::~Transform() {}

Vec3 Transform::GetRight()
{
	Vector rightVec = ::XMVector3TransformNormal({ 1, 0, 0 }, _world);
	
	Vec3 right;
	::XMStoreFloat3(&right, rightVec);
	return right;
}

Vec3 Transform::GetUp()
{
	Vector upVec = ::XMVector3TransformNormal({ 0, 1, 0 }, _world);

	Vec3 up;
	::XMStoreFloat3(&up, upVec);
	return up;
}

Vec3 Transform::GetForward()
{
	Vector forwardVec = ::XMVector3TransformNormal({ 0, 0, 1 }, _world);

	Vec3 forward;
	::XMStoreFloat3(&forward, forwardVec);
	return forward;
}

Vec3 Transform::GetWorldPosition()
{
	Vector posVec;
	::XMMatrixDecompose(&posVec, nullptr, nullptr, _world);

	Vec3 pos;
	::XMStoreFloat3(&pos, posVec);
	return pos;
}

Vec3 Transform::GetWorldRotation()
{
	return Vec3();
}

Vec3 Transform::GetWorldScale()
{
	return Vec3();
}

void Transform::SetWorldPosition(const Vec3& position)
{
	if (HasParent())
	{
		Matrix parentWorldMatrix = _parent->GetWorldMatrix(); 
		Vector localPositionVector = ::XMVector3TransformCoord(::XMLoadFloat3(&position), ::XMMatrixInverse(nullptr, parentWorldMatrix));
		
		Vec3 localPosition;
		::XMStoreFloat3(&localPosition, localPositionVector);
		
		SetLocalPosition(localPosition);
	}
	else
	{
		SetLocalPosition(position);
	}
}

void Transform::SetWorldRotation(const Vec3& rotation)
{
	if (HasParent())
	{
		Matrix parentWorldMatrix = _parent->GetWorldMatrix();
		Vector localRotationVector = ::XMVector3TransformNormal(::XMLoadFloat3(&rotation), ::XMMatrixInverse(nullptr, parentWorldMatrix));
		
		Vec3 localRotation;
		::XMStoreFloat3(&localRotation, localRotationVector);

		SetLocalRotation(localRotation);
	}
	else
	{
		SetLocalRotation(rotation);
	}
}

void Transform::SetWorldScale(const Vec3& scale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetWorldScale();
		Vec3 newScale = scale;
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

// root들 만을 대상으로 한다, 자식의 매트릭스까지 수행한다.
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