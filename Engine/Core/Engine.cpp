#include "Engine.h"
#include "Input.h"
#include <iostream>

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	instance = this;
	input = std::make_shared<Input>();
	LoadEngineSettings();

	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	srand(static_cast<unsigned int>(time(nullptr)));
}

Engine::~Engine() { }

Engine& Engine::Get()
{
	return *instance;
}

void Engine::Run()
{
	LARGE_INTEGER current_time;
	LARGE_INTEGER previous_time;
	QueryPerformanceCounter(&current_time);
	previous_time = current_time;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	float target_framerate = (settings.framerate <= 0.0f) ? 60.0f : settings.framerate;
	float oneframe_time = 1.0f / target_framerate;

	while (true)
	{
		QueryPerformanceCounter(&current_time);
		float delta_time = (current_time.QuadPart - previous_time.QuadPart) / (float)frequency.QuadPart;

		input->ProcessInput();

		if (delta_time >= oneframe_time)
		{
			Awake();
			Update(delta_time);
			Render();

			char title_text[50] = { };
			sprintf_s(title_text, 50, "FPS : %f", (1.0f / delta_time));
			SetConsoleTitleA(title_text);

			previous_time = current_time;

			input->SavePreviousKeyStates();
		}
	}
}

void Engine::Quit()
{
	PostQuitMessage(0);
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

void Engine::Awake()
{

}

void Engine::Update(float delta_time)
{

}

void Engine::Render()
{

}