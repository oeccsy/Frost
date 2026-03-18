#pragma once
#include "Object/Object.h"

class Triangle : public Object
{
    RTTI_DECLARATIONS(Triangle, Object)
    
public:
    Triangle();
    virtual ~Triangle();
    
    virtual void Awake() override;
    virtual void Update(float delta_time) override;
    
protected:
    void CreateGeometry();
    void CreateShader();
    void CreateInputLayout();
};
