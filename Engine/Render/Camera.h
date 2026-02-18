#pragma once

#include "Core.h"
#include "Enum/ProjectionType.h"
#include "Data/Frustum.h"
#include "Types.h"
#include <memory>

class ENGINE_API Camera
{
public:
    Camera();
    virtual ~Camera();

    void LateUpdate();

    FORCEINLINE shared_ptr<class Transform> GetTransform() const { return transform; }
    FORCEINLINE Matrix GetViewMatrix() const { return view_matrix; }
    FORCEINLINE Matrix GetProjMatrix() const { return proj_matrix; }
    FORCEINLINE Matrix GetViewProjMatrix() const { return view_matrix * proj_matrix; }

    void SetProjectionType(ProjectionType new_type);
    void SetFrustum(Frustum new_frustum);
    
    static shared_ptr<Camera> GetMainCamera() { return main_camera; }
    static void SetMainCamera(shared_ptr<Camera> camera) { main_camera = camera; }

private :
    void CalculateViewMatrix();
    void CalculateProjMatrix();

private:
    ProjectionType projection_type;
    Frustum frustum;

    shared_ptr<class Transform> transform;
    Matrix view_matrix;
    Matrix proj_matrix;
    
    static shared_ptr<Camera> main_camera;
};