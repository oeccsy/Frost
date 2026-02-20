#pragma once

#include "Core.h"
#include "Component/Collider/Collider.h"
#include "RTTI.h"
#include "Types.h"

struct Circle3D;
struct Triangle3D;
struct Plane3D;

class ENGINE_API MeshCollider : public Collider
{
	RTTI_DECLARATIONS(MeshCollider, Collider);
	
public:
	MeshCollider();
	virtual ~MeshCollider();

	virtual void Awake() override;

	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;
	bool Intersects(Circle3D& circle, OUT float& theta);
	Vector3 Snap(Vector3 position);

private:
	weak_ptr<class Mesh> mesh;
	shared_ptr<class TriangleOctree> triangle_octree;
};