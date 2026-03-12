#pragma once

#include "Component/Component.h"
#include <unordered_set>

class Frost : public Component
{
	RTTI_DECLARATIONS(Frost, Component)
	
public:
	Frost();
	virtual ~Frost() override;
	
	void Awake() override;
	void Update() override;

	void ForkRandomRoots();

protected:
	void GrowBranches();
	void ForkBranches();
	void ForkCloseRoots();
	void StopCloseBranches();
	void UpdateFrostPoints();
	void UpdateGrowingBranches();

public:
	static const float MAIN_BRANCH_GROW_STEP;
	static const float SUB_BRANCH_GROW_STEP;
	static const float MAIN_BRANCH_STOP_DIST;
	static const float SUB_BRANCH_STOP_DIST;
	static const float ROOT_FORK_DIST;
	static const int MAX_BRANCHES_PER_FORK;

private:
	shared_ptr<class MeshCollider> guide_mesh_collider;
	shared_ptr<class PointCloud> frost_points;

	vector<shared_ptr<class FrostRoot>> unforked_frost_roots;
	vector<shared_ptr<class FrostRoot>> forked_frost_roots;
};