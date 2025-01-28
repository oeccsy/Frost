#include "pch.h"
#include "Object.h"
#include "Sphere.h"
#include "PointCloud.h"
#include "Mesh.h"
#include "Frost.h"
#include <random>

Frost::Frost()
{
	sphere = make_shared<Sphere>();
	pointCloud = make_shared<PointCloud>(sphere->GetMesh());
}

Frost::~Frost() {}

void Frost::Render(shared_ptr<Renderer> renderer)
{
	renderer->Render(sphere->GetMesh(), sphere->GetMaterial(), sphere->GetTransform());
	renderer->Render(pointCloud->GetMesh(), pointCloud->GetMaterial(), pointCloud->GetTransform());
}

void Frost::SelectStartPoints()
{
	float threshold = 0.1f;
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	
	for (auto point : pointCloud->GetMesh()->GetVertices())
	{
		if (dis(gen) < 0.1f)
		{
			startPoints.push_back(point);
		}
	}
}

// PointCloud
bool Frost::FindCloseSeed()
{
	return true;
}

void Frost::CreateGeometry()
{

}

void Frost::CreateShader()
{
}

void Frost::CreateInputLayout()
{
}