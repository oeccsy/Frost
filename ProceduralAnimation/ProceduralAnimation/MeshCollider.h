#pragma once

class Mesh;

class MeshCollider : public Collider
{
public:
	MeshCollider();
	virtual ~MeshCollider();

public:
	bool Intersects(Ray& ray, OUT Point3D& hitPoint);
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

private:
	bool Raycast(const Plane3D& plane, const Ray& ray, OUT Point3D& hitPoint);
	bool Raycast(const Triangle3D& triangle, const Ray& ray, OUT Point3D& hitPoint);
	bool IsPointInPlane(const Point3D& point, const Plane3D& plane);
	bool IsPointInTriangle(const Point3D& p, const Triangle3D& t);

private:
	weak_ptr<Mesh> _mesh;
};