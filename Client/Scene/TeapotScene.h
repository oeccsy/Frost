#pragma once

#include "Scene/Scene.h"

class TeapotScene : public Scene
{
    RTTI_DECLARATIONS(TeapotScene, Scene)

public:
    TeapotScene();
    virtual ~TeapotScene();

    virtual void Awake() override;
};
