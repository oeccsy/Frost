#pragma once

#include "Core.h"
#include "Data/EngineSettings.h"
#include <memory>

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Init(HWND hwnd);
	void Run();
	void Quit();

	FORCEINLINE int GetWidth() const { return settings.width; }
	FORCEINLINE int GetHeight() const { return settings.height; }
	FORCEINLINE shared_ptr<class Scene> GetScene() const { return scene; }
	void SetScene(shared_ptr<class Scene> new_scene);
	
	static Engine& Get();

private:
	void LoadEngineSettings();
	
private:
	EngineSettings settings;
	
	shared_ptr<class Timer> timer;
	shared_ptr<class Input> input;
	shared_ptr<class Graphics> graphics;
	shared_ptr<class Scene> scene;
	
	static Engine* instance;
};