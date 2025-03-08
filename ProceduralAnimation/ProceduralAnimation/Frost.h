#pragma once

class Sphere;
class PointCloud;
class FrostVectorField;

class FrostRoot;
class FrostMainBranch;

class Frost : public Object
{
public:
	Frost();
	virtual ~Frost() override;

public:
	void Update() override;
	void Render(shared_ptr<Renderer> renderer) override;

public:
	void StartFrostAnim();
	shared_ptr<Sphere> GetSphere() { return _sphere; }

protected:
	void ForkRandomRoots();

protected:
	void Grow();

private:
	shared_ptr<Sphere> _sphere;
	shared_ptr<PointCloud> _basePoints;
	shared_ptr<PointCloud> _refPoints;
	shared_ptr<FrostVectorField> _vectorField;

	vector<FrostRoot> _frostRoots;

	vector<shared_ptr<Vertex>> _unforkedPoints; // TODO : ForkRandomRoots()에서 unforked, forked 분리, 가까운 핵 감지
	vector<shared_ptr<Vertex>> _forkedPoints;
};