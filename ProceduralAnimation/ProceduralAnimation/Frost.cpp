#include "pch.h"
#include "Object.h"
#include "Sphere.h"
#include "PointCloud.h"
#include "VectorField.h"
#include "FrostVectorField.h"
#include "FrostRoot.h"
#include "FrostMainBranch.h"
#include "Mesh.h"
#include "PointOctree.h"
#include <random>
#include "Frost.h"

const float Frost::MIN_POINT_DIST = 0.5f;

Frost::Frost()
{
	_sphere = make_shared<Sphere>();
	_basePoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::MeshVertices, 100);
	_refPoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::Scatter, 100);
	_vectorField = make_shared<FrostVectorField>(_basePoints);

	for (auto& basePoint : _basePoints->GetPoints())
	{
		_unforkedFrostRoots.insert(make_shared<FrostRoot>(*this, basePoint));
	}

	_branchPoints = make_unique<PointOctree>(BoundingBox({ Vector3(0, 0, 0), Vector3(6, 6, 6) })); // TODO : 위치 이동 반영
}

Frost::~Frost() {}

void Frost::Update()
{
	Object::Update();
	Grow();
}

void Frost::Render(shared_ptr<Renderer> renderer)
{
	renderer->Render(_sphere->GetMesh(), _sphere->GetMaterial(), _sphere->GetTransform());
	// renderer->Render(_basePoints->GetMesh(), _basePoints->GetMaterial(), _basePoints->GetTransform());
	// renderer->Render(_refPoints->GetMesh(), _refPoints->GetMaterial(), _refPoints->GetTransform());
	
	for (auto frostRoot : _forkedFrostRoots)
	{
		for (auto mainBranch : frostRoot->GetGrowingBranches())
		{
			renderer->Render(mainBranch->GetMesh(), mainBranch->GetMaterial(), mainBranch->GetTransform());
		}
	}
}

void Frost::StartFrostAnim()
{
	ForkRandomRoots();
}

void Frost::ForkRandomRoots()
{
	/*float threshold = 0.05f;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	
	for (FrostRoot& root : _frostRoots)
	{
		if (root.IsForked()) continue;
		if (dis(gen) < threshold) root.Fork();
	}*/

	for (auto root : _unforkedFrostRoots)
	{
		root->Fork();
		_unforkedFrostRoots.erase(root);
		_forkedFrostRoots.insert(root);

		break;
	}
}

void Frost::Grow()
{
	for (auto& frostRoot : _forkedFrostRoots)
	{
		auto& branches = frostRoot->GetGrowingBranches();
		

		for (auto it = branches.begin(); it != branches.end();)
		{
			auto branch = *it;
			branch->Grow();

			BoundingSphere checkBounds({ branch->GetBranchEndPos(), MIN_POINT_DIST });

			if (_branchPoints->IntersectsWithPoints(checkBounds))
			{
				//frostRoot->StopGrowing(branch);
				it = branches.erase(it);
			}
			else
			{
				it++;
			}

			_branchPoints->Insert(branch->GetBranchEndPos());
		}
	}
}
