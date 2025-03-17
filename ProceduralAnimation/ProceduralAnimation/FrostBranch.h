#pragma once

class FrostRoot;
class MeshCollider;

class FrostBranch : public Object
{
public:
	FrostBranch(Vector3& basePoint, Vector3& dir, Vector3& normal, shared_ptr<FrostBranch> parent);
	virtual ~FrostBranch() override;

public:
	bool Grow(shared_ptr<MeshCollider> target);
	bool Fork(shared_ptr<MeshCollider> target);

public:
	shared_ptr<FrostBranch>& GetParent() { return _parent; }
	vector<shared_ptr<FrostBranch>>& GetChildren() { return _children; }
	Vector3 GetBranchEndPos() { return _branch.back().position; };

private:
	Circle3D _guideCircle;

	vector<Vertex> _branch;

	shared_ptr<FrostBranch> _parent;
	vector<shared_ptr<FrostBranch>> _children;
};