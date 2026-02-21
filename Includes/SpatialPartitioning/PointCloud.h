#pragma once

#include "Core.h"
#include "Data/Vertex.h"
#include "Types.h"
#include <memory>

class PointCloud
{
public:
	PointCloud(const BoundingBox bounds);
	virtual ~PointCloud();

	void GenerateMeshVertices(shared_ptr<class Mesh> base_mesh, int amount);
	void GenerateMeshTriangleCenters(shared_ptr<class Mesh> base_mesh, int amount);
	void GenerateScatterPoints(shared_ptr<class Mesh> base_mesh, int amount);
	
	Vector3 GetNearPoint(Vector3 pos);
	vector<Vector3> GetAllPoints();
	bool IntersectsWithBounds(const BoundingSphere& bounding_sphere);
	bool IntersectsWithPoints(const BoundingSphere& bounding_sphere);

protected:
	FORCEINLINE bool IsLeaf() const { return children[0] == nullptr; }
	void Insert(Vector3 point);
	void Subdivide();
	bool IsInBounds(Vector3 position);

protected:
	BoundingBox bounds;
	shared_ptr<PointCloud> children[8] = { nullptr };

	vector<Vector3> points;
	static const int MAX_POINTS_PER_SECTION = 16;
};

