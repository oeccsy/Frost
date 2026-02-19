#pragma once

#include "Types.h"

struct TransformData
{
    Matrix worldMatrix = Matrix::Identity;
    Matrix viewMatrix = Matrix::Identity;;
    Matrix projMatrix = Matrix::Identity;;
};