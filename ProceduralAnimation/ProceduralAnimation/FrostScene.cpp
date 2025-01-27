#include "pch.h"
#include "Scene.h"
#include "FrostScene.h"
#include "Object.h"
#include "Sphere.h"
#include "Scatter.h"
#include "Camera.h"
#include "Transform.h"
#include "Component.h"

FrostScene::FrostScene() {}

FrostScene::~FrostScene() {}

void FrostScene::Init()
{
	_camera = make_shared<Camera>();
	Camera::SetMainCamera(_camera);
	Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vec3(0, 0, -10));

	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	_objects.push_back(sphere);
	shared_ptr<Scatter> scatter = make_shared<Scatter>(sphere->GetMesh());
	_objects.push_back(scatter);
}
