#pragma once

class VectorField
{
public:
	VectorField();
	virtual ~VectorField();

public:
	virtual Vector3 GetVector(Vector3 pos) = 0;
};