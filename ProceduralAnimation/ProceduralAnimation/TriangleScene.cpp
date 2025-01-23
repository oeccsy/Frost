#include "pch.h"
#include "Scene.h"
#include "TriangleScene.h"
#include "Object.h"
#include "Triangle.h"
#include "Cube.h"
#include "Camera.h"

TriangleScene::TriangleScene() {}

TriangleScene::~TriangleScene() {}

void TriangleScene::Init()
{
	_camera = make_shared<Camera>();
	Camera::SetCamera(_camera);
	_objects.push_back(make_shared<Triangle>());
}