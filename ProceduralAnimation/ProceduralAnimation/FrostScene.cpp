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
#include "TeaPot.h"
#include "BufferPool.h"

FrostScene::FrostScene() {}

FrostScene::~FrostScene() {}

void FrostScene::Init()
{
	_camera = make_shared<Camera>();
	Camera::SetMainCamera(_camera);
	Camera::GetMainCamera()->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));

	BufferPool::vertexBufferPool = make_unique<BufferPool>(BufferType::Vertex, sizeof(Vertex) * 200, 1200);
	BufferPool::indexBufferPool = make_unique<BufferPool>(BufferType::Index, sizeof(uint32) * 200, 1200);

	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	{
		sphere->AddComponent(make_shared<MeshCollider>());
		sphere->AddComponent(make_shared<Frost>());
	}

	_objects.push_back(sphere);

	sphere->GetComponent<Frost>()->ForkRandomRoots();

	//shared_ptr<TeaPot> teapot = make_shared<TeaPot>();
	//{
	//	teapot->GetTransform()->SetWorldRotation(Vector3(-XM_PI / 6, 0, 0));
	//	
	//	teapot->AddComponent(make_shared<MeshCollider>());
	//	teapot->AddComponent(make_shared<Frost>());
	//}

	//_objects.push_back(teapot);

	//teapot->GetComponent<Frost>()->ForkRandomRoots();
}
