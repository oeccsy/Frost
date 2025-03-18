#include "pch.h"
#include "Object.h"
#include "Sphere.h"
#include "PointCloud.h"
#include "VectorField.h"
#include "FrostVectorField.h"
#include "FrostRoot.h"
#include "FrostBranch.h"
#include "Mesh.h"
#include "PointOctree.h"
#include "Component.h"
#include "Collider.h"
#include "MeshCollider.h"
#include <random>
#include "Frost.h"

const float Frost::MAIN_BRANCH_GROW_SPEED = 0.2f;
const float Frost::SUB_BRANCH_GROW_SPEED = 0.04f;
const float Frost::ROOT_FORK_DIST = 0.5f;
const float Frost::MIN_POINT_DIST = 0.f;

Frost::Frost()
{
	_sphere = make_shared<Sphere>();
	_basePoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::Scatter, 30);
	_refPoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::Scatter, 30);
	_vectorField = make_shared<FrostVectorField>(_basePoints);

	_unforkedRootsOctree = _frostPointsOctree = make_shared<PointOctree>(BoundingBox({ Vector3(0, 0, 0), Vector3(6, 6, 6) }));
	_frostPointsOctree = make_shared<PointOctree>(BoundingBox({ Vector3(0, 0, 0), Vector3(6, 6, 6) })); // TODO : 위치 이동 반영

	for (auto& basePoint : _basePoints->GetPoints())
	{
		shared_ptr<FrostRoot> newRoot = make_shared<FrostRoot>(basePoint.position, basePoint.normal);
		_unforkedFrostRoots.insert(newRoot);
		_unforkedRootsOctree->Insert(newRoot->GetBasePoint());
	}
}

Frost::~Frost() {}

void Frost::Update()
{
	Object::Update();
	Grow();
	ForkCloseRoots();
}

void Frost::Render(shared_ptr<Renderer> renderer)
{
	// renderer->Render(_sphere->GetMesh(), _sphere->GetMaterial(), _sphere->GetTransform());
	// renderer->Render(_basePoints->GetMesh(), _basePoints->GetMaterial(), _basePoints->GetTransform());
	// renderer->Render(_refPoints->GetMesh(), _refPoints->GetMaterial(), _refPoints->GetTransform());
	
	for (auto frostRoot : _forkedFrostRoots)
	{
		for (auto mainBranch : frostRoot->GetBranches())
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
		root->ForkRoot(_sphere->GetComponent<MeshCollider>());
		
		_frostPointsOctree->Insert(root->GetBasePoint());

		_unforkedFrostRoots.erase(root);
		_forkedFrostRoots.insert(root);

		break;
	}


}

void Frost::ForkCloseRoots()
{
	for (auto it = _unforkedFrostRoots.begin(); it != _unforkedFrostRoots.end();)
	{
		auto root = *it; 

		BoundingSphere checkBounds({ root->GetBasePoint(), ROOT_FORK_DIST });

		if (_frostPointsOctree->IntersectsWithPoints(checkBounds))
		{
			root->ForkRoot(_sphere->GetComponent<MeshCollider>());

			_frostPointsOctree->Insert(root->GetBasePoint());

			it = _unforkedFrostRoots.erase(it);
			_forkedFrostRoots.insert(root);
		}
		else
		{
			it++;
		}
	}
}

void Frost::Grow()
{
	auto target = _sphere->GetComponent<MeshCollider>();

	for (auto root : _forkedFrostRoots)
	{
		root->GrowBranches(target);
		root->ForkMainBranches(target);

		vector<Vector3> endPoints = root->GetLatestEndPoints();
		root->DisableGrowth(_frostPointsOctree);
		
		for (Vector3& point : endPoints)
		{
			_frostPointsOctree->Insert(point);
		}
	}
}