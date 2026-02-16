#pragma once

class Sphere : public Object
{
public:
	Sphere();
	~Sphere() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};