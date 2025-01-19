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
	Matrix matWorld = DirectX::XMMatrixIdentity();
	Matrix matView = DirectX::XMMatrixIdentity();
	Matrix matProjection = DirectX::XMMatrixIdentity();
};