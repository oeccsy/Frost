#pragma once

class FrostRoot;

class FrostMainBranch : public Object
{
public:
	FrostMainBranch(Vertex& basePoint, Vector3& dir, Vector3& normal, FrostRoot& parent);
	virtual ~FrostMainBranch() override;

public:
	FrostRoot& GetParent() { return _parent; }
	Vector3 GetBranchEndPos() { return _branch.back().position; };
	void Grow();

private:
	static const float GROW_SPEED;
	Circle3D _guideCircle;
	
	FrostRoot& _parent;
	vector<Vertex> _branch;
};