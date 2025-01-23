#pragma once

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

class Camera
{
public:
	Camera();
	virtual ~Camera();

	static shared_ptr<Camera> GetMainCamera() { return _mainCamera; }
	static void SetMainCamera(shared_ptr<Camera> camera) { _mainCamera = camera; }

	shared_ptr<Transform> GetTransform() { return _transform; }

	Matrix GetViewMatrix() { return _view; }
	Matrix GetProjMatrix() { return _proj; }
	Matrix GetViewProjMatrix() { return _view * _proj; }

	void SetProjectionType(ProjectionType type);
	void SetFrustum(Frustum frustum);

	void LateUpdate();

private :
	void CalculateViewMatrix();
	void CalculateProjMatrix();

private:
	static shared_ptr<Camera> _mainCamera;

	shared_ptr<Transform> _transform;

	ProjectionType _type;
	Frustum _frustum;

	Matrix _view;
	Matrix _proj;
};