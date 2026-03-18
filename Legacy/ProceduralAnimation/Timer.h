#pragma once

class Timer
{
public:
	static void Init();
	static void Update();
	
	__forceinline static float GetDeltaTime() { return deltaTime; }

private:
	static LARGE_INTEGER cpuFrequency;
	static LARGE_INTEGER prevFrequency;
	static LARGE_INTEGER currentFrequency;
	static float deltaTime;
};
