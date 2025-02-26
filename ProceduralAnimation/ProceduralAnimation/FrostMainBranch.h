#pragma once

class FrostMainBranch : public Object
{
public:
	FrostMainBranch(Vertex& basePoint, Vector3& dir);
	virtual ~FrostMainBranch() override;

public:
	Vector3 GetBranchEndPos() { return _branch.back().position; };
	void GrowTo(Vector3 dir);

private:
	static const float GROW_SPEED;

	vector<Vertex> _branch;
};