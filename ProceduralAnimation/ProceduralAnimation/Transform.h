#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

public:
	Vec3 GetRight();
	Vec3 GetUp();
	Vec3 GetForward();

public:
	Vec3 GetLocalPosition() { return _localPosition; }
	Vec3 GetLocalRotation() { return _localRotation; }
	Vec3 GetLocalScale() { return _localScale; }
	Vec3 GetWorldPosition() { return _worldPosition; }
	Vec3 GetWorldRotation() { return _worldRotation; }
	Vec3 GetWorldScale() { return _worldScale; }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; CalculateMatrix(); }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; CalculateMatrix();	}
	void SetLocalScale(const Vec3& scale) { _localScale = scale; CalculateMatrix();	}
	void SetWorldPosition(const Vec3& position);
	void SetWorldRotation(const Vec3& rotation);
	void SetWorldScale(const Vec3& scale);

	void CalculateMatrix();
	Matrix GetWorldMatrix() { return _world; }

public:
	bool HasParent() { return _parent != nullptr; }

	shared_ptr<Transform> GetParent() { return _parent; }
	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }

	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
	Vec3 _worldPosition = { 0.f, 0.f, 0.f };
	Vec3 _worldRotation = { 0.f, 0.f, 0.f };
	Vec3 _worldScale = { 1.f, 1.f, 1.f };

	Matrix _local = ::XMMatrixIdentity();
	Matrix _world = ::XMMatrixIdentity();

	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};