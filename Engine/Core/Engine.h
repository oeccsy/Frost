#pragma once

#include "Core.h"
#include "EngineSettings.h"
#include <memory>

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();
	void Quit();

	FORCEINLINE int Width() const { return settings.width; }
	FORCEINLINE int Height() const { return settings.height; }

	static Engine& Get();

private:
	void LoadEngineSettings();

	void Awake();
	void Update(float delta_time = 0.0f);
	void Render();
	
private:
	EngineSettings settings;
	std::shared_ptr<class Input> input = nullptr;
	
	static Engine* instance;
};