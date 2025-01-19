#pragma once
class Object;

class Triangle : public Object
{
public:
	Triangle();
	~Triangle() override;

protected:
	void CreateGeometry() override;
	void CreateVS() override;
	void CreatePS() override;
	void CreateInputLayout() override;
};