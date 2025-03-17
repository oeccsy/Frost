#pragma once

class FrostBranch;
class Frost;

class MeshCollider;
class PointOctree;

class FrostRoot
{
public:
	FrostRoot(Vector3 basePoint, Vector3 normal);
	~FrostRoot();

public:	
	void ForkRoot(shared_ptr<MeshCollider> target);

public:
	void GrowBranches(shared_ptr<MeshCollider> target);
	void ForkMainBranches(shared_ptr<MeshCollider> target);
	void DisableGrowth(shared_ptr<PointOctree> target);
	vector<Vector3> GetLatestEndPoints();

public:
	Vector3& GetBasePoint() { return _basePoint; }
	vector<shared_ptr<FrostBranch>>& GetBranches() { return _branches; }
	unordered_set<shared_ptr<FrostBranch>>& GetGrowingBranches() { return _growingBranches; }

private:
	Vector3 _basePoint;
	Vector3 _normal;

	vector<shared_ptr<FrostBranch>> _branches;
	unordered_set<shared_ptr<FrostBranch>> _growingBranches;
	unordered_set<shared_ptr<FrostBranch>> _growingMainBranches;
};