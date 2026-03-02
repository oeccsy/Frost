#include "pch.h"
#include "FrostScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"
#include "Object/Teapot.h"
#include "Component/Collider/MeshCollider.h"
#include "Component/Frost.h"

FrostScene::FrostScene() { }

FrostScene::~FrostScene() {}

void FrostScene::Awake()
{
    Scene::Awake();
    
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 3, 10));
    Camera::GetMainCamera()->GetTransform()->SetWorldRotation(Vector3(-10, 0, 0));

    shared_ptr<Teapot> teapot = SpawnObject<Teapot>();
    teapot->AddComponent(make_shared<MeshCollider>());
    teapot->AddComponent(make_shared<Frost>());
    
    teapot->GetComponent<Frost>()->ForkRandomRoots();
}
