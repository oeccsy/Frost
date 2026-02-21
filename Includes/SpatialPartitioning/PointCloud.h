#pragma once

#include "Core.h"
#include "Data/Vertex.h"
#include "Types.h"
#include <memory>

enum class PointType
{
	MeshVertices,
	TriangleCenter,
	Scatter
};

class PointCloud
{
public:
	PointCloud(const BoundingBox bounds);
	virtual ~PointCloud() override;

	void GenerateMeshVertices(shared_ptr<class Mesh> base_mesh, int amount);
	void GenerateMeshTriangleCenters(shared_ptr<class Mesh> base_mesh, int amount);
	void GenerateScatterPoints(shared_ptr<class Mesh> base_mesh, int amount);

protected:
	FORCEINLINE bool IsLeaf() const { children[0] == nullptr; }
	void Insert(Vector3 point);
	void Subdivide();
	bool IsInBounds(Vector3 position);

protected:
	BoundingBox bounds;
	shared_ptr<PointCloud> children[8] = { nullptr };

	vector<Vector3> points;
	static const int MAX_POINTS_PER_SECTION = 10;

public:
	bool IntersectsWithBounds(const BoundingSphere& boundingSphere);
	bool IntersectsWithPoints(const BoundingSphere& boundingSphere);

public:
	vector<Vertex>& GetPoints() { return points; }
	Vector3 GetNearPoint(Vector3 pos);

protected:
	void CreateScatterPoints();
};

