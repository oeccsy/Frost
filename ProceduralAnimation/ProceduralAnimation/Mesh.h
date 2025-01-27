#pragma once

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

public:
	vector<Vertex>& GetVertices() { return _vertices; }
	vector<uint32>& GetIndices() { return _indices; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return _topology; }

	void SetVertices(vector<Vertex>& vertices) { _vertices = vertices; }
	void SetIndices(vector<uint32>& indices) { _indices = indices; }
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { _topology = topology; }

public:
	void CreateBuffers();
	void CreateInputLayout(shared_ptr<Material> material);
	
private:
	friend class Renderer;

	// IA
	vector<Vertex> _vertices;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	ComPtr<ID3D11InputLayout> _inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY _topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};