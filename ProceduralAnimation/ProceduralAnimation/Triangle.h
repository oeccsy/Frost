#pragma once

class Object;
class Mesh;
class Material;

class Triangle : public Object
{
public:
	Triangle();
	~Triangle() override;
	void Update() override;
	void Render(shared_ptr<Pipeline> pipeline) override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
	void CreateContantBuffer();
};