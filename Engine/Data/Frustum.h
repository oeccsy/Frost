#pragma once

struct Frustum
{
    float near_z;
    float far_z;
    float aspect;
    float fov_y;

    float far_width;
    float far_height;
};
