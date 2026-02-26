#pragma once
#include "Object/Object.h"

class Teapot : public Object
{
    RTTI_DECLARATIONS(Teapot, Object)

public:
    Teapot();
    virtual ~Teapot();

    virtual void Awake() override;
    virtual void Update(float delta_time) override;

protected:
    void CreateGeometry();
    void CreateShader();
    void CreateInputLayout();
};
