#pragma once

class TeaPot : public Object
{
public:
	TeaPot();
	virtual ~TeaPot() override;

protected:
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();
};