#include "pch.h"
#include "App.h"
#include "Scene.h"
#include "Object.h"

App::App() {}

App::~App() {}

void App::Init(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);
	_pipeline = make_shared<Pipeline>();
}

int App::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!currentScene) continue;
			
			auto& objects = currentScene->GetObjects();
			
			for (auto& object : objects)
			{
				object->Update();
			}
			
			_graphics->RenderBegin();
			
			for (auto& object : objects)
			{
				object->Render(_pipeline);
			}

			_graphics->RenderEnd();
		}
	}

	return (int)msg.wParam;
}

void App::LoadScene(Scene* newScene)
{
	if (currentScene) delete currentScene;
	currentScene = newScene;
	currentScene->Init();
}
