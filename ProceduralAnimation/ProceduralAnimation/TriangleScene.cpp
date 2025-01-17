#include "pch.h"
#include "Scene.h"
#include "TriangleScene.h"
#include "Object.h"
#include "Triangle.h"

TriangleScene::TriangleScene() {}

TriangleScene::~TriangleScene() {}

void TriangleScene::Init()
{
	objects.push_back(new Triangle());
}