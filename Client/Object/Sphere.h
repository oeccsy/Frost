#pragma once

#include "Object/Object.h"

class Sphere : public Object
{
    RTTI_DECLARATIONS(Sphere, Object)

public:
    Sphere();
    virtual ~Sphere();
    
    virtual void Awake() override;
    virtual void Update(float delta_time) override;
    
protected:
    void CreateGeometry();
    void CreateShader();
    void CreateInputLayout();
};
