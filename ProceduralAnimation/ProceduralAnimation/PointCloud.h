#pragma once

class PointCloud : public Object
{
public:
	PointCloud(shared_ptr<Mesh> mesh);
	PointCloud(shared_ptr<Mesh> mesh, int pointCount);
	~PointCloud() override;

public:
	void Update() override;

protected:
	void CreateGeometry(shared_ptr<Mesh> mesh);
	void CreateGeometry(shared_ptr<Mesh> mesh, int pointCount);
	void CreateShader();
	void CreateInputLayout();
	Vec3 GetCenterPos(Vec3 a, Vec3 b, Vec3 c);
	Vec3 GetRandomPosInTriangle(Vec3 a, Vec3 b, Vec3 c);

protected:
	int _pointCount;
};