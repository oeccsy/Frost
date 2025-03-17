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

const float Frost::MAIN_BRANCH_GROW_SPEED = 0.5f;
const float Frost::SUB_BRANCH_GROW_SPEED = 0.1f;
const float Frost::MIN_POINT_DIST = 0.f;

Frost::Frost()
{
	_sphere = make_shared<Sphere>();
	_basePoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::MeshVertices, 100);
	_refPoints = make_shared<PointCloud>(_sphere->GetMesh(), PointType::Scatter, 100);
	_vectorField = make_shared<FrostVectorField>(_basePoints);

	for (auto& basePoint : _basePoints->GetPoints())
	{
		_unforkedFrostRoots.insert(make_shared<FrostRoot>(basePoint.position, basePoint.normal));
	}

	_pointsContainer = make_shared<PointOctree>(BoundingBox({ Vector3(0, 0, 0), Vector3(6, 6, 6) })); // TODO : 위치 이동 반영
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
		
		_pointsContainer->Insert(root->GetBasePoint());

		_unforkedFrostRoots.erase(root);
		_forkedFrostRoots.insert(root);

		break;
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
		root->DisableGrowth(_pointsContainer);
		
		for (Vector3& point : endPoints)
		{
			_pointsContainer->Insert(point);
		}
	}
}