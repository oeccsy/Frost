#pragma once

#include "Core.h"

class ENGINE_API Input
{
	friend class Engine;

	struct KeyState
	{
		bool is_key_down = false;
		bool previouse_key_down = false;
	};

public:
	Input();

	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static Input& Get();

private:
	void ProcessInput();
	void SavePreviousKeyStates();

private:
	KeyState key_states[255] = { };

	static Input* instance;
};