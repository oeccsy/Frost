#include "pch.h"
#include "SphereScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"
#include "Render/Renderer/Renderer.h"

SphereScene::SphereScene() { }

SphereScene::~SphereScene() { }

void SphereScene::Awake()
{
    Scene::Awake();

    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 3, 10));
    Camera::GetMainCamera()->GetTransform()->SetWorldRotation(Vector3(-10, 0, 0));

    shared_ptr<Sphere> sphere = SpawnObject<Sphere>();
    sphere->AddComponent(make_shared<Renderer>());
}


