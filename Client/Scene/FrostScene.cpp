#include "pch.h"
#include "FrostScene.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Object/Sphere.h"
#include "Component/Collider/MeshCollider.h"
#include "Component/Frost.h"

FrostScene::FrostScene() { }

FrostScene::~FrostScene() {}

void FrostScene::Awake()
{
    Scene::Awake();
    
    Camera::SetMainCamera(make_shared<Camera>());
    Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, 10));

    shared_ptr<Sphere> sphere = make_shared<Sphere>();
    sphere->Awake();
    sphere->AddComponent(make_shared<MeshCollider>());
    sphere->AddComponent(make_shared<Frost>());
    AddObject(sphere);
    
    sphere->GetComponent<Frost>()->ForkRandomRoots();
}
