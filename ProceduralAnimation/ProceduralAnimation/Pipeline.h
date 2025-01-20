#pragma once

class Mesh;
class Material;

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void Render(shared_ptr<Mesh> mesh, shared_ptr<Material> material);
};