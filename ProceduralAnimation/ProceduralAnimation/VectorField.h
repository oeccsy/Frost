#pragma once

class VectorField
{
public:
	VectorField();
	virtual ~VectorField();

public:
	virtual Vec3 GetVector(Vec3 pos) = 0;
};