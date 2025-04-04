#include "pch.h"
#include "Transform.h"
#include "Camera.h"

shared_ptr<Camera> Camera::_mainCamera = nullptr;

Camera::Camera()
{
	_transform = make_shared<Transform>();

	_type = ProjectionType::Perspective;

	_frustum.nearZ = 1.f;
	_frustum.farZ = 100.f;
	_frustum.aspect = (float) GWinSizeX / GWinSizeY;
	_frustum.fovY = XM_2PI / 6.0f;
	
	_frustum.farWidth = GWinSizeX;
	_frustum.farHeight = GWinSizeY;

	_view = ::XMMatrixIdentity();
	_proj = ::XMMatrixIdentity();

	CalculateViewMatrix();
	CalculateProjMatrix();
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

void Camera::LateUpdate()
{
	CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
	Vector3 pos = _transform->GetWorldPosition();
	Vector3 forward = _transform->GetForward();
	Vector3 up = _transform->GetUp();

	XMVECTOR posVector = ::XMLoadFloat3(&pos);
	XMVECTOR forwardVector = ::XMLoadFloat3(&forward);
	XMVECTOR upVector = ::XMLoadFloat3(&up);
	
	_view = ::XMMatrixLookToLH(posVector, forwardVector, upVector);
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