#pragma once

#include "Scene/Scene.h"

class TriangleScene : public Scene
{
    RTTI_DECLARATIONS(TriangleScene, Scene)
    
public:
    TriangleScene();
    virtual ~TriangleScene() override;
    
    virtual void Awake() override;
};
