#include "pch.h"
#include "App.h"
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "TriangleScene.h"
#include "Timer.h"

App::App() {}

App::~App() {}

void App::Init(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);
	_renderer = make_shared<Renderer>();

    Timer::Init();
    
    _currentScene = make_shared<TriangleScene>();
    _currentScene->Init();
}

void App::Run()
{
    if (!_currentScene) return;
    
    Timer::Update();

    _currentScene->Update();
    _currentScene->LateUpdate();

    _graphics->RenderBegin();
    _currentScene->Render(_renderer);
    _graphics->RenderEnd();
}