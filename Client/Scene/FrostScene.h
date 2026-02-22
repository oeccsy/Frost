#pragma once

#include "Scene/Scene.h"

class FrostScene : public Scene
{
    RTTI_DECLARATIONS(FrostScene, Scene)

public:
    FrostScene();
    virtual ~FrostScene();
    
    virtual void Awake() override;
};
