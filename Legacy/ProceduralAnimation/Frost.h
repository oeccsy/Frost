#pragma once

class MeshCollider;
class PointCloud;
class FrostRoot;
class PointOctree;

class Frost : public Component
{
public:
	Frost();
	virtual ~Frost() override;

public:
	void Init() override;
	void Update() override;
	void Render(shared_ptr<Renderer> renderer) override;

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
	shared_ptr<MeshCollider> _guideMeshCollider;
	shared_ptr<PointCloud> _basePoints;

	shared_ptr<PointOctree> _unforkedRootsOctree;
	shared_ptr<PointOctree> _frostPointsOctree;

	unordered_set<shared_ptr<FrostRoot>> _unforkedFrostRoots;
	unordered_set<shared_ptr<FrostRoot>> _forkedFrostRoots;
};