#include "pch.h"
#include "TriangleScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Triangle.h"
#include "Render/Renderer/Renderer.h"

TriangleScene::TriangleScene() { }

TriangleScene::~TriangleScene() { }

void TriangleScene::Awake()
{
    Scene::Awake();
    
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, 10));
    Camera::GetMainCamera()->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));
    
    shared_ptr<Triangle> triangle = SpawnObject<Triangle>();
    triangle->AddComponent(make_shared<Renderer>());
}
