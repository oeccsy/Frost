#pragma once

class Mesh;
class TriangleOctree;

class MeshCollider : public Collider
{
public:
	MeshCollider();
	virtual ~MeshCollider();

public:
	void Init() override;

public:
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;
	bool Intersects(Circle3D& circle, OUT float& theta);
	Vector3 Snap(Vector3 position);

private:
	bool Raycast(const Plane3D& plane, const Ray& ray, OUT float& distance);
	bool Raycast(const Triangle3D& triangle, const Ray& ray, OUT float& distance);
	bool Circlecast(const Triangle3D& triangle, const Circle3D& circle, OUT vector<float>& theta);
	bool IsPointInPlane(const Point3D& point, const Plane3D& plane);
	bool IsPointInTriangle(const Point3D& point, const Triangle3D& triangle);
	
private:
	weak_ptr<Mesh> _mesh;
	shared_ptr<TriangleOctree> _triangleOctree;
};