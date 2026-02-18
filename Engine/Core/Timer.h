#pragma once

#include "Core.h"

class ENGINE_API Timer
{
public:
    Timer();
    virtual ~Timer();
    
    float CalculateDeltaTime();
    void Update();
    
private:
    LARGE_INTEGER current_time;
    LARGE_INTEGER previous_time;
    LARGE_INTEGER frequency;
    
    float delta_time;
};
