#include "Engine.h"
#include "Timer.h"
#include "Input.h"
#include "Graphics.h"
#include "Scene/Scene.h"
#include <iostream>

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	LoadEngineSettings();
	
	timer = make_shared<Timer>();
	input = make_shared<Input>();
	graphics = make_shared<Graphics>();
	
	instance = this;
	srand(static_cast<unsigned int>(time(nullptr)));
}

Engine::~Engine() { }

void Engine::Init(HWND hwnd)
{
	graphics->Init(hwnd, settings.width, settings.height);
}

void Engine::Run()
{
	float target_framerate = settings.framerate == 0.0f ? 60.0f : settings.framerate;
	float one_frame_time = 1.0f / target_framerate;

	MSG msg = { };
	
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float delta_time = timer->CalculateDeltaTime();
			input->ProcessInput();

			if (scene == nullptr) continue;
		
			if (delta_time >= one_frame_time)
			{
				timer->Update();
			
				scene->Awake();
				scene->Start();
				scene->Update(delta_time);
				scene->LateUpdate();
			
				graphics->RenderBegin();
				scene->Render();
				graphics->RenderEnd();
			
				input->SavePreviousKeyStates();

				scene->ProcessAddAndDestroyObjects();
			}
		}
	}
}

void Engine::Quit()
{
	PostQuitMessage(0);
}

Engine& Engine::Get()
{
	return *instance;
}

void Engine::LoadEngineSettings()
{
	FILE* file = nullptr;
	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");

	if (file == nullptr)
	{
		std::cout << "Failed to load engine settings." << '\n';
		__debugbreak();
		return;
	}

	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = new char[file_size + 1];
	memset(buffer, 0, file_size + 1);

	size_t read_size = fread(buffer, sizeof(char), file_size, file);

	char* context = nullptr;
	char* token = nullptr;
	token = strtok_s(buffer, "\n", &context);

	while (token != nullptr)
	{
		char header[10] = { };
		sscanf_s(token, "%s", header, 10);

		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}

		token = strtok_s(nullptr, "\n", &context);
	}

	delete[] buffer;
	buffer = nullptr;

	fclose(file);
}