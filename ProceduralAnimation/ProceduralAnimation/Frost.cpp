#include "pch.h"
#include "Object.h"
#include "Sphere.h"
#include "PointCloud.h"
#include "VectorField.h"
#include "FrostVectorField.h"
#include "FrostRoot.h"
#include "FrostMainBranch.h"
#include "Mesh.h"
#include <random>
#include "Frost.h"

Frost::Frost()
{
	_sphere = make_shared<Sphere>();
	_basePoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::MeshVertices, 100);
	_refPoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::Scatter, 100);
	_vectorField = make_shared<FrostVectorField>(_basePoints);

	for (auto& basePoint : _basePoints->GetPoints())
	{
		_frostRoots.emplace_back(*this, basePoint);
	}
}

Frost::~Frost() {}

void Frost::Update()
{
	Object::Update();
}

void Frost::Render(shared_ptr<Renderer> renderer)
{
	renderer->Render(_sphere->GetMesh(), _sphere->GetMaterial(), _sphere->GetTransform());
	// renderer->Render(_basePoints->GetMesh(), _basePoints->GetMaterial(), _basePoints->GetTransform());
	// renderer->Render(_refPoints->GetMesh(), _refPoints->GetMaterial(), _refPoints->GetTransform());
	
	for (auto frostRoot : _frostRoots)
	{
		for (auto mainBranch : frostRoot.GetMainBranches())
		{
			renderer->Render(mainBranch.GetMesh(), mainBranch.GetMaterial(), mainBranch.GetTransform());
		}
	}
}

void Frost::StartFrostAnim()
{
	ForkRandomRoots();
	
	for (int i = 0; i < 10; i++)
	{
		Grow();
	}
}

void Frost::ForkRandomRoots()
{
	float threshold = 0.05f;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	
	for (FrostRoot& root : _frostRoots)
	{
		if (root.IsForked()) continue;
		if (dis(gen) < threshold) root.Fork();
	}
}

void Frost::Grow()
{
	for (auto& frostRoot : _frostRoots)
	{
		for (auto& mainBranch : frostRoot.GetMainBranches())
		{
			mainBranch.Grow();
		}
	}
}
