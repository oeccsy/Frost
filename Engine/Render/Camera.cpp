#include "Camera.h"
#include "Transform.h"
#include "Engine.h"

shared_ptr<Camera> Camera::main_camera = nullptr;

Camera::Camera()
{
    transform = make_shared<Transform>();

    projection_type = ProjectionType::Perspective;

    frustum.near_z = 1.f;
    frustum.far_z = 100.f;
    frustum.aspect = static_cast<float>(Engine::Get().Width()) / Engine::Get().Height();
    frustum.fov_y = XM_2PI / 6.0f;
	
    frustum.far_width = static_cast<float>(Engine::Get().Width());
    frustum.far_height = static_cast<float>(Engine::Get().Height());

    view_matrix = Matrix::Identity;
    proj_matrix = Matrix::Identity;

    CalculateViewMatrix();
    CalculateProjMatrix();
}

Camera::~Camera() { }

void Camera::SetProjectionType(ProjectionType new_type)
{
    projection_type = new_type;
    CalculateProjMatrix();
}

void Camera::SetFrustum(Frustum new_frustum)
{
    frustum = new_frustum;
    CalculateProjMatrix();
}

void Camera::LateUpdate()
{
    CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
    Vector3 eye = transform->GetWorldPosition();
    Vector3 forward = transform->GetForward();
    Vector3 target = eye + forward;
    Vector3 up = transform->GetUp();
    
    view_matrix = Matrix::CreateLookAt(eye, target, up);
}

void Camera::CalculateProjMatrix()
{
    if (projection_type == ProjectionType::Perspective)
    {
        proj_matrix = Matrix::CreatePerspectiveFieldOfView(frustum.fov_y, frustum.aspect, frustum.near_z, frustum.far_z);
    }
    else
    {
        proj_matrix = Matrix::CreateOrthographic(frustum.far_width, frustum.far_height, frustum.near_z, frustum.far_z);
    }
}