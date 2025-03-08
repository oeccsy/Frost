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
	vector<Vertex>& GetPoints() { return _points; }
	Vector3 GetNearPoint(Vector3 pos);

protected:
	void CreateGeometry();
	void CreateMeshVertices();
	void CreateTriangleCenterPoints();
	void CreateScatterPoints();
	void CreateShader();
	void CreateInputLayout();
	Vector3 GetCenterPos(Vector3 a, Vector3 b, Vector3 c);
	Vector3 GetRandomPosInTriangle(Vector3 a, Vector3 b, Vector3 c);

protected:
	vector<Vertex> _points;

	shared_ptr<Mesh> _baseMesh;
	PointType _pointType = PointType::MeshVertices;
	int _pointCount;
};