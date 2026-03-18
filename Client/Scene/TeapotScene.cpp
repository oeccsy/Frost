#include "pch.h"
#include "TeapotScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Teapot.h"
#include "Render/Renderer/Renderer.h"

TeapotScene::TeapotScene() {}

TeapotScene::~TeapotScene() {}

void TeapotScene::Awake()
{
    Scene::Awake();

    Camera::SetMainCamera(make_shared<Camera>());
	Camera::SetMainCamera(make_shared<Camera>());
	Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 3, 10));
	Camera::GetMainCamera()->GetTransform()->SetWorldRotation(Vector3(-10, 0, 0));

    shared_ptr<Teapot> teapot = SpawnObject<Teapot>();
    teapot->AddComponent(make_shared<Renderer>());
}


