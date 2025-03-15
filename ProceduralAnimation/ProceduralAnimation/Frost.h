#pragma once

class Sphere;
class PointCloud;
class FrostVectorField;

class FrostRoot;
class FrostMainBranch;

class PointOctree;

class Frost : public Object
{
public:
	Frost();
	virtual ~Frost() override;

public:
	void Update() override;
	void Render(shared_ptr<Renderer> renderer) override;

public:
	void StartFrostAnim();
	shared_ptr<Sphere> GetSphere() { return _sphere; }

protected:
	void ForkRandomRoots();
	void Grow();

private:
	static const float MIN_POINT_DIST;

	shared_ptr<Sphere> _sphere;
	shared_ptr<PointCloud> _basePoints;
	shared_ptr<PointCloud> _refPoints;
	shared_ptr<FrostVectorField> _vectorField;

	unordered_set<shared_ptr<FrostRoot>> _unforkedFrostRoots;
	unordered_set<shared_ptr<FrostRoot>> _forkedFrostRoots;

	unique_ptr<PointOctree> _branchPoints;
};