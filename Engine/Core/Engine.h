#pragma once

#include "Core.h"
#include "EngineSettings.h"
#include <memory>

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	static Engine& Get();

	FORCEINLINE int Width() const { return settings.width; }
	FORCEINLINE int Height() const { return settings.height; }

	void Run();
	void Quit();

private:
	void LoadEngineSettings();

	void Awake();
	void Update(float delta_time = 0.0f);
	void Render();
	
private:
	static Engine* instance;

	std::shared_ptr<class Input> input = nullptr;
	EngineSettings settings;
};