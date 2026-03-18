#include "pch.h"
#include <windows.h>
#include "Timer.h"

LARGE_INTEGER Timer::cpuFrequency = {};
LARGE_INTEGER Timer::prevFrequency = {};
LARGE_INTEGER Timer::currentFrequency = {};

float Timer::deltaTime = 0.0f;

void Timer::Init()
{
	QueryPerformanceFrequency(&cpuFrequency);
	QueryPerformanceCounter(&prevFrequency);
}

void Timer::Update()
{
	QueryPerformanceCounter(&currentFrequency);
	float differenceFrequency = static_cast<float>(currentFrequency.QuadPart - prevFrequency.QuadPart);
	deltaTime = differenceFrequency / static_cast<float>(cpuFrequency.QuadPart);
	prevFrequency.QuadPart = currentFrequency.QuadPart;
}
