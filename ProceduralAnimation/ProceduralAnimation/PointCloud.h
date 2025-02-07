#pragma once

enum class PointType
{
	MeshVertices,
	TriangleCenter,
	Scatter
};

class PointCloud : public Object
{
public:
	PointCloud(shared_ptr<Mesh> mesh, PointType pointType, int pointCount);
	~PointCloud() override;

public:
	void Update() override;

public:
	vector<Vertex>& GetPoints();
	Vec3 GetNearPoint(Vec3 pos);

protected:
	void CreateGeometry();
	void CreateMeshVertices();
	void CreateTriangleCenterPoints();
	void CreateScatterPoints();
	void CreateShader();
	void CreateInputLayout();
	Vec3 GetCenterPos(Vec3 a, Vec3 b, Vec3 c);
	Vec3 GetRandomPosInTriangle(Vec3 a, Vec3 b, Vec3 c);

protected:
	vector<Vertex> _points;

	shared_ptr<Mesh> _baseMesh;
	PointType _pointType = PointType::MeshVertices;
	int _pointCount;
};