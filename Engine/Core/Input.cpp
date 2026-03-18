#include "Input.h"
#include <Windows.h>

Input* Input::instance = nullptr;

Input::Input()
{
	instance = this;
}

void Input::ProcessInput()
{
	for (int i = 0; i < 255; ++i)
	{
		key_states[i].is_key_down = GetAsyncKeyState(i) & 0x8000;
	}
}

void Input::SavePreviousKeyStates()
{
	for (int i = 0; i < 255; ++i)
	{
		key_states[i].previouse_key_down = key_states[i].is_key_down;
	}
}

bool Input::GetKey(int keyCode)
{
	return key_states[keyCode].is_key_down;
}

bool Input::GetKeyDown(int keyCode)
{
	return !key_states[keyCode].previouse_key_down && key_states[keyCode].is_key_down;
}

bool Input::GetKeyUp(int keyCode)
{
	return key_states[keyCode].previouse_key_down && !key_states[keyCode].is_key_down;
}

Input& Input::Get()
{
	return *instance;
}