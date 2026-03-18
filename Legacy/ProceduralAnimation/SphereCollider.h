#pragma once

class SphereCollider : public Collider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

public:
	virtual void Update() override;
	virtual bool Intersects(Ray&, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<Collider>& other) override;

	BoundingSphere& GetBoundingSphere() { return _boundingSphere; }

	void SetRadius(float radius) { _radius = radius; }

private:
	float _radius = 1.0f;
	BoundingSphere _boundingSphere;
};