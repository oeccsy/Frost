#pragma once

#include "Types.h"

struct TransformData
{
    Matrix world_matrix = Matrix::Identity;
    Matrix view_matrix = Matrix::Identity;;
    Matrix proj_matrix = Matrix::Identity;;
};