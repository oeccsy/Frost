#pragma once

class Sphere;
class PointCloud;
class FrostVectorField;

class FrostRoot;
class FrostBranch;

class MeshCollider;
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
	shared_ptr<Sphere> GetSphere() { return _sphere; }

public:
	void ForkRandomRoots();

protected:
	void Grow();
	void ForkCloseRoots();

public:
	static const float MAIN_BRANCH_GROW_SPEED;
	static const float SUB_BRANCH_GROW_SPEED;
	static const float ROOT_FORK_DIST;
	static const float MIN_POINT_DIST;
	static const int MAX_BRANCH_COUNT = 6;

private:
	shared_ptr<Sphere> _sphere;
	shared_ptr<PointCloud> _basePoints;
	shared_ptr<PointCloud> _refPoints;
	shared_ptr<FrostVectorField> _vectorField;

	unordered_set<shared_ptr<FrostRoot>> _unforkedFrostRoots;
	unordered_set<shared_ptr<FrostRoot>> _forkedFrostRoots;

	shared_ptr<PointOctree> _unforkedRootsOctree;
	shared_ptr<PointOctree> _frostPointsOctree;
};