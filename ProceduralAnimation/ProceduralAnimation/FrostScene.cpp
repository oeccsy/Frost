#include "pch.h"
#include "Scene.h"
#include "FrostScene.h"
#include "Object.h"
#include "Frost.h"

FrostScene::FrostScene() {}

FrostScene::~FrostScene() {}

void FrostScene::Init()
{
	objects.push_back(new Frost());
}