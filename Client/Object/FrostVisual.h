#pragma once

#include "Object/Object.h"

class FrostVisual : public Object
{
    RTTI_DECLARATIONS(FrostVisual, Object)

public:
    FrostVisual();
    virtual ~FrostVisual() override;

    virtual void Awake() override;
    virtual void Update(float delta_time) override;

protected:
    void CreateGeometry();
    void CreateShader();
    void CreateInputLayout();
};
