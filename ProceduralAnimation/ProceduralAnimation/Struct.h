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
	Matrix worldMatrix = DirectX::XMMatrixIdentity();
	Matrix viewMatrix = DirectX::XMMatrixIdentity();
	Matrix projMatrix = DirectX::XMMatrixIdentity();
};