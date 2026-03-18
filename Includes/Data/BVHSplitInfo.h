#pragma once

#include "Core.h"
#include <memory>

struct ENGINE_API BVHSplitInfo
{
	int axis;
	float split_value;
	float cost;
};