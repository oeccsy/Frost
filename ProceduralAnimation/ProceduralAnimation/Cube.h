#pragma once

class Object;
class Mesh;
class Material;

class Cube : public Object
{
public:
	Cube();
	~Cube() override;
	void Update() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};