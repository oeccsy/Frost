#pragma once

#include "Core.h"
#include "Types.h"

struct ENGINE_API Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;
};