#pragma once

class FrostMainBranch;

class FrostRoot
{
public:
	FrostRoot(Vertex& basePoint);
	~FrostRoot();

public:	
	bool IsForked() { return _isForked; }
	void Fork();
	void Grow();

public:
	vector<FrostMainBranch>& GetMainBranches() { return _mainBranches; }

private:
	static const int MAX_BRANCH_COUNT = 6;

	Vertex* _basePoint;
	vector<FrostMainBranch> _mainBranches;
	bool _isForked;
};