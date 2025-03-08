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
	vector<FrostMainBranch>& GetMainBranches() { return _mainBranches; }

private:
	static const int MAX_BRANCH_COUNT = 6;

	Frost& _parent;

	Vertex& _basePoint;
	vector<FrostMainBranch> _mainBranches;
	bool _isForked;
};