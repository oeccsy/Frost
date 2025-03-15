#pragma once

class FrostMainBranch;
class Frost;

class FrostRoot
{
public:
	FrostRoot(Frost& parent, Vertex& basePoint);
	~FrostRoot();

public:	
	bool IsForked() { return _isForked; }
	void Fork();

public:
	Frost& GetParent() { return _parent; }
	vector<shared_ptr<FrostMainBranch>>& GetBranches() { return _branches; }
	unordered_set<shared_ptr<FrostMainBranch>>& GetGrowingBranches() { return _growingBranches; }

private:
	static const int MAX_BRANCH_COUNT = 6;

	Frost& _parent;

	Vertex& _basePoint;

	vector<shared_ptr<FrostMainBranch>> _branches;
	unordered_set<shared_ptr<FrostMainBranch>> _growingBranches;

	bool _isForked;
};