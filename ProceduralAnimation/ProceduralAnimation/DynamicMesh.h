#pragma once

class DynamicMesh : public Mesh
{
public:
	DynamicMesh();
	virtual ~DynamicMesh();

public:
	void CreateBuffers() override;
	void UpdateBuffers() override;
};