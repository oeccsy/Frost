#include "pch.h"
#include "Scene.h"
#include "TriangleScene.h"
#include "Object.h"
#include "Triangle.h"
#include "Rectangle.h"

TriangleScene::TriangleScene() {}

TriangleScene::~TriangleScene() {}

void TriangleScene::Init()
{
	//objects.push_back(new Triangle());
	objects.push_back(make_shared<class Rectangle>());
}