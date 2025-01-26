#pragma once

class Sphere : public Object
{
public:
	Sphere();
	~Sphere() override;

public:
	void Update() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};