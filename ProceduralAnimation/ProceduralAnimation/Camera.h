#pragma once

class Object;
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

	static shared_ptr<Camera> GetCamera() { return _camera; }
	static void SetCamera(shared_ptr<Camera> camera) { _camera = camera; }

	Matrix GetViewMatrix() { return _view; }
	Matrix GetProjMatrix() { return _proj; }
	Matrix GetViewProjMatrix() { return _view * _proj; }

	void SetProjectionType(ProjectionType type);
	void SetFrustum(Frustum frustum);

	void Update();

private :
	void CalculateViewMatrix();
	void CalculateProjMatrix();

private:
	static shared_ptr<Camera> _camera;

	shared_ptr<Transform> _transform;

	ProjectionType _type;
	Frustum _frustum;

	Matrix _view;
	Matrix _proj;
};