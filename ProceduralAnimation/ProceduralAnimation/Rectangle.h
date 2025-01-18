#pragma once
class Object;

class Rectangle : public Object
{
public:
	Rectangle();
	~Rectangle() override;
	void Update() override;
	void Render() override;

protected:
	void CreateGeometry() override;
	void CreateVS() override;
	void CreatePS() override;
	void CreateInputLayout() override;
};