#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 position;
	Vec2 uv;
	Color color;
};

struct TransformData
{
	Vec3 offset;
	float dummy; // 상수 버퍼의 16byte 정렬을 위한 변수
};