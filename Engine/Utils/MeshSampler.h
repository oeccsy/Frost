#pragma once

#include "Core.h"
#include "Data/Vertex.h"
#include <memory>
#include <vector>

class ENGINE_API MeshSampler
{
public:
    MeshSampler() = delete;
    ~MeshSampler() = delete;
    
    static vector<Vertex> GenerateMeshVertices(shared_ptr<class Mesh> base_mesh, int amount);
    static vector<Vertex> GenerateMeshTriangleCenters(shared_ptr<class Mesh> base_mesh, int amount);
    static vector<Vertex> GenerateScatterPoints(shared_ptr<class Mesh> base_mesh, int amount);
};
