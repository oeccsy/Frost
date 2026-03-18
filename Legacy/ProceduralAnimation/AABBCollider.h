#pragma once

class AABBCollider : Collider
{
public:
	AABBCollider() : Collider(ColliderType::AABB) {}
	virtual ~AABBCollider();
	
public:
	virtual void Update() override;
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

	BoundingBox& GetBoundingBox() { return _boundingBox; }

private:
	BoundingBox _boundingBox;
};