#pragma once

class DynamicMesh : public Mesh
{
public:
	DynamicMesh();
	virtual ~DynamicMesh();

public:
	void CreateBuffers() override;
	void UpdateBuffers() override;

private:
	bool _useVertexBufferPool = false;
	bool _useIndexBufferPool = false;
};