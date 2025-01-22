#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"

Camera::Camera()
{
	_type = ProjectionType::Perspective;

	_frustum.nearZ = 1.f;
	_frustum.farZ = 100.f;
	_frustum.aspect = 800.f / 600.f;
	_frustum.fovY = XM_PIDIV2;
	_frustum.farWidth = 800.f;
	_frustum.farHeight = 600.f;

	_view = ::XMMatrixIdentity();
	_proj = ::XMMatrixIdentity();
}

Camera::~Camera() {}

void Camera::SetProjectionType(ProjectionType type)
{
	_type = type;
	CalculateProjMatrix();
}

void Camera::SetFrustum(Frustum frustum)
{
	_frustum = frustum;
	CalculateProjMatrix();
}

void Camera::Update()
{
	CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
	Vec3 pos = _transform.GetWorldPosition();
	Vec3 forward = _transform.GetForward();
	Vec3 up = _transform.GetUp();

	XMVECTOR posVector = ::XMLoadFloat3(&pos);
	XMVECTOR forwardVector = ::XMLoadFloat3(&forward);
	XMVECTOR upVector = ::XMLoadFloat3(&up);
	
	_view = ::XMMatrixLookAtLH(posVector, XMVectorAdd(posVector, forwardVector), upVector);
}

void Camera::CalculateProjMatrix()
{
	if (_type == ProjectionType::Perspective)
	{
		_proj = ::XMMatrixPerspectiveFovLH(_frustum.fovY, _frustum.aspect, _frustum.nearZ, _frustum.farZ);
	}
	else
	{
		_proj = ::XMMatrixOrthographicLH(_frustum.farWidth, _frustum.farHeight, _frustum.nearZ, _frustum.farZ);
	}
}