#pragma once

#include "Core.h"
#include "Vertex.h"
#include "Types.h"
#include <vector>
#include <memory>

class ENGINE_API Mesh
{
public:
	Mesh();
	virtual ~Mesh();

public:
	FORCEINLINE const vector<Vertex>& GetVertices() const { return vertices; }
	FORCEINLINE const vector<uint32>& GetIndices() const { return indices; }
	FORCEINLINE D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return topology; }

	FORCEINLINE void SetVertices(vector<Vertex>& vertices) { vertices = vertices; }
	FORCEINLINE void SetIndices(vector<uint32>& indices) { indices = indices; }
	FORCEINLINE void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { topology = topology; }

public:
	virtual void CreateBuffers();
	virtual void UpdateBuffers();
	void CreateInputLayout(shared_ptr<class Material> material);

protected:
	// IA
	vector<Vertex> vertices;
	vector<uint32> indices;
	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;
	ComPtr<ID3D11InputLayout> input_layout;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};