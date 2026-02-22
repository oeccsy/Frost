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
	void Grow();
	void ForkCloseRoots();

public:
	static const float MAIN_BRANCH_GROW_SPEED;
	static const float SUB_BRANCH_GROW_SPEED;
	static const float ROOT_FORK_DIST;
	static const float MIN_POINT_DIST;
	static const int MAX_BRANCH_COUNT = 6;

private:
	shared_ptr<class MeshCollider> guide_mesh_collider;
	shared_ptr<class PointCloud> base_points;
	
	shared_ptr<class PointCloud> unforked_roots;
	shared_ptr<class PointCloud> frost_points;

	unordered_set<shared_ptr<class FrostRoot>> unforked_frost_roots;
	unordered_set<shared_ptr<class FrostRoot>> forked_frost_roots;
};