#include "pch.h"
#include "TriangleScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"

TriangleScene::TriangleScene()
{
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
}

TriangleScene::~TriangleScene() { }
