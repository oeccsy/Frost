#pragma once

class PointCloud;

class FrostVectorField : public VectorField
{
public:
	FrostVectorField(shared_ptr<PointCloud> pointCloud);
	virtual ~FrostVectorField() override;

public:
	Vec3 GetVector(Vec3 pos) override;

private:
	weak_ptr<PointCloud> _refPoints;
};