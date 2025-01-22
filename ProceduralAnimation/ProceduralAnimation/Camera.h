#pragma once
#include "Component.h"

class Transform;

enum class ProjectionType
{
	Perspective,
	Orthographic,
};

struct Frustum
{
	float nearZ;
	float farZ;
	float aspect;
	float fovY;

	float farWidth;
	float farHeight;
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	Matrix GetViewProjMatrix() { return _view * _proj; }

	void SetProjectionType(ProjectionType type);
	void SetFrustum(Frustum frustum);

	virtual void Update() override;

private :
	void CalculateViewMatrix();
	void CalculateProjMatrix();

private:
	ProjectionType _type;
	
	Transform _transform;
	Frustum _frustum;

	Matrix _view;
	Matrix _proj;
};