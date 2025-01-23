#pragma once

class Object;
class Mesh;
class Material;

class Triangle : public Object
{
public:
	Triangle();
	~Triangle() override;

public:
	void Update() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};