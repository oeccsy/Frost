#pragma once

class Scatter : public Object
{
public:
	Scatter(shared_ptr<Mesh> mesh);
	~Scatter() override;

public:
	void Update() override;

protected:
	void CreateGeometry(shared_ptr<Mesh> mesh);
	void CreateShader();
	void CreateInputLayout();
	Vec3 GetRandomPosInTriangle(Vec3 a, Vec3 b, Vec3 c);
};