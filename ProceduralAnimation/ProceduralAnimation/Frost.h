#pragma once

class Sphere;
class PointCloud;

class Frost : public Object
{
public:
	Frost();
	virtual ~Frost() override;

public:
	void Render(shared_ptr<Renderer> renderer) override;

protected:
	void SelectStartPoints();
	bool FindCloseSeed();
	void CreateGeometry();
	void CreateShader();
	void CreateInputLayout();

private:
	shared_ptr<Sphere> sphere;
	shared_ptr<PointCloud> pointCloud;
	vector<Vertex> startPoints;
};