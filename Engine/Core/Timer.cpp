#include "Timer.h"

Timer::Timer()
{
    QueryPerformanceCounter(&current_time);
    previous_time = current_time;
    QueryPerformanceFrequency(&frequency);
    
    delta_time = 0.0f;
}

Timer::~Timer() { }

float Timer::CalculateDeltaTime()
{
    QueryPerformanceCounter(&current_time);
    delta_time = static_cast<float>(current_time.QuadPart - previous_time.QuadPart) / frequency.QuadPart;
    return delta_time;
}

void Timer::Update()
{
    previous_time = current_time;
}