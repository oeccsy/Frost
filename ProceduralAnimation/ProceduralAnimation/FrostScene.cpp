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

	shared_ptr<Frost> frost = make_shared<Frost>();
	{
		frost->GetSphere()->AddComponent(make_shared<MeshCollider>());
	}

	_objects.push_back(frost);

	frost->StartFrostAnim();
}
