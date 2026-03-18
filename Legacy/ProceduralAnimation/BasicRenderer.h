#pragma once

class BasicRenderer : public Renderer
{
public:
	BasicRenderer();
	virtual ~BasicRenderer();

public:
	void Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material, shared_ptr<Transform> transform) override;
};