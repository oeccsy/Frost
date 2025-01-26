#include "pch.h"
#include "Scene.h"
#include "TriangleScene.h"
#include "Object.h"
#include "Triangle.h"
#include "Cube.h"
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

	shared_ptr<Cube> cube = make_shared<Cube>();
	_objects.push_back(cube);
	cube->GetTransform()->SetWorldRotation(Vec3(0.5f, 0.5f, 0.5f));
}
