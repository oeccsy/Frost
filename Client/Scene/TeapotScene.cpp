#include "pch.h"
#include "TeapotScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"
#include "Object/Teapot.h"

TeapotScene::TeapotScene() {}

TeapotScene::~TeapotScene() {}

void TeapotScene::Awake()
{
    Scene::Awake();

    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, 10));

    shared_ptr<Teapot> teapot = make_shared<Teapot>();
    teapot->Awake();
    teapot->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
	teapot->GetTransform()->SetWorldRotation(Vector3(30, 0, 0));
    AddObject(teapot);
}


