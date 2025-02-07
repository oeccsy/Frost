#pragma once

class FrostMainBranch : public Object
{
public:
	FrostMainBranch(Vertex& basePoint, Vec3& dir);
	virtual ~FrostMainBranch() override;

public:
	Vec3 GetBranchEndPos() { return _branch.back().position; };
	void GrowTo(Vec3 dir);

private:
	static const float GROW_SPEED;

	vector<Vertex> _branch;
};