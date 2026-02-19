#include "pch.h"
#include "TriangleScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Triangle.h"

TriangleScene::TriangleScene()
{
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
    
    shared_ptr<Triangle> triangle = make_shared<Triangle>();
    AddObject(triangle);
}

TriangleScene::~TriangleScene() { }