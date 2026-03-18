#pragma once

class OBBCollider : public Collider
{
public:
	OBBCollider() : Collider(ColliderType::OBB) {}
	virtual ~OBBCollider();

public:
	virtual void Update() override;
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;
	
	BoundingOrientedBox& GetBoundingBox() { return _boundingBox; }

private:
	BoundingOrientedBox _boundingBox;
};