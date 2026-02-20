#include "Transform.h"
#include "Types.h"
#include <cmath>

using namespace DirectX;

Transform::Transform() {}

Transform::~Transform() {}

void Transform::SetLocalPosition(const Vector3& position)
{
	local_position = position;

	RefreshMatrix();
	RefreshWorldTRS();
}

void Transform::SetLocalRotation(const Quaternion& rotation)
{
	local_rotation = rotation;

	RefreshMatrix();
	RefreshWorldTRS();
}

void Transform::SetLocalScale(const Vector3& scale)
{
	local_scale = scale;

	RefreshMatrix();
	RefreshWorldTRS();
}

void Transform::SetWorldPosition(const Vector3& position)
{
	shared_ptr<Transform> parent_transform = parent.lock();
	
	if (parent_transform == nullptr)
	{
		SetLocalPosition(position);
	}
	else
	{
		Matrix world_to_parent_local = parent_transform->GetWorldMatrix().Invert();
		SetLocalPosition(Vector3::Transform(position, world_to_parent_local));
	}
}

void Transform::SetWorldRotation(const Quaternion& rotation)
{
	shared_ptr<Transform> parent_transform = parent.lock();
	
	if (parent_transform == nullptr)
	{
		SetLocalRotation(rotation);
	}
	else
	{
		Quaternion parent_world_rot = parent_transform->GetWorldRotation();
		Quaternion inv_parent;
		parent_world_rot.Inverse(inv_parent);
		
		SetLocalRotation(rotation * inv_parent);
	}
}

void Transform::SetWorldScale(const Vector3& scale)
{
	shared_ptr<Transform> parent_transform = parent.lock();
	
	if (parent_transform == nullptr)
	{
		SetLocalScale(scale);
	}
	else
	{
		Vector3 parent_scale = parent_transform->GetWorldScale();
		SetLocalScale(Vector3(scale.x / parent_scale.x, scale.y / parent_scale.y, scale.z / parent_scale.z));
	}
}

void Transform::RefreshMatrix()
{
	// SRT
	Matrix s = Matrix::CreateScale(local_scale);
	Matrix r = Matrix::CreateFromQuaternion(local_rotation);
	Matrix t = Matrix::CreateTranslation(local_position);
	
	shared_ptr<Transform> parent_transform = parent.lock();
	local_matrix = s * r * t;
	world_matrix = (parent_transform == nullptr) ? local_matrix : local_matrix * parent_transform->GetWorldMatrix();

	for (const shared_ptr<Transform>& child : children)
	{
		child->RefreshMatrix();
		child->RefreshWorldTRS();
	}
}

Vector3 ConvertToEuler(Quaternion quaternion)
{
	Vector3 angles;
	
	// roll (x-axis)
	double sinr_cosp = 2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	double cosr_cosp = 1 - 2 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	angles.x = static_cast<float>(std::atan2(sinr_cosp, cosr_cosp));

	// pitch (y-axis)
	double sinp = std::sqrt(1 + 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	double cosp = std::sqrt(1 - 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	angles.y = static_cast<float>(2 * std::atan2(sinp, cosp) - XM_PI / 2);

	// yaw (z-axis)
	double siny_cosp = 2 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	double cosy_cosp = 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	angles.z = static_cast<FLOAT>(std::atan2(siny_cosp, cosy_cosp));

	return angles;
}

void Transform::RefreshWorldTRS()
{
	world_matrix.Decompose(world_scale, world_rotation, world_position);
}