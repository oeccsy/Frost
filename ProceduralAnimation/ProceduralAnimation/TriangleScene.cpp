#include "pch.h"
#include "Scene.h"
#include "TriangleScene.h"
#include "Object.h"
#include "Triangle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Camera.h"
#include "Transform.h"
#include "Component.h"

TriangleScene::TriangleScene() {}

TriangleScene::~TriangleScene() {}

void TriangleScene::Init()
{
	_camera = make_shared<Camera>();
	Camera::SetMainCamera(_camera);
	Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vec3(0, 0, -10));

	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	_objects.push_back(sphere);
	sphere->GetTransform()->SetWorldRotation(Vec3(0.5f, 0.5f, 0.5f));
}
