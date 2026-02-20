#include "pch.h"
#include "SphereScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"

SphereScene::SphereScene()
{
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, 10));
    
    shared_ptr<Sphere> sphere = make_shared<Sphere>();
    sphere->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
    AddObject(sphere);
}

SphereScene::~SphereScene() { }
