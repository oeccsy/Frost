#pragma once

class Cube : public Object
{
public:
	Cube();
	~Cube() override;

public:
	void Update() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};