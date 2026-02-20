#include "pch.h"
#include "SphereScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"

SphereScene::SphereScene()
{
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, 30));
    
    shared_ptr<Sphere> sphere = make_shared<Sphere>();
    AddObject(sphere);
}

SphereScene::~SphereScene() { }
