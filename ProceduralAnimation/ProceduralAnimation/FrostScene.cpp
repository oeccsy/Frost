#include "pch.h"
#include "Scene.h"
#include "FrostScene.h"
#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "Component.h"
#include "Frost.h"
#include "Collider.h"
#include "Mesh.h"
#include "MeshCollider.h"
#include "Sphere.h"

FrostScene::FrostScene() {}

FrostScene::~FrostScene() {}

void FrostScene::Init()
{
	_camera = make_shared<Camera>();
	Camera::SetMainCamera(_camera);
	Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));

	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	{
		sphere->AddComponent(make_shared<MeshCollider>());
		sphere->AddComponent(make_shared<Frost>());
	}

	_objects.push_back(sphere);

	sphere->GetComponent<Frost>()->ForkRandomRoots();
}
