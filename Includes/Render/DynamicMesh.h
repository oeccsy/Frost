#pragma once

#include "Render/Mesh.h"
#include "Core.h"
#include "RTTI.h"

class ENGINE_API DynamicMesh : public Mesh
{
    RTTI_DECLARATIONS(DynamicMesh, Mesh)
    
public:
    DynamicMesh();
    virtual ~DynamicMesh() override;

    void CreateBuffers() override;
    void UpdateBuffers() override;
};