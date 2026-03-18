#pragma once

class Transform
{
public:
	Transform();
	virtual ~Transform();

public:
	Vector3 GetRight();
	Vector3 GetUp();
	Vector3 GetForward();

public:
	Vector3 GetLocalPosition() { return _localPosition; }
	Vector3 GetLocalRotation() { return _localRotation; }
	Vector3 GetLocalScale() { return _localScale; }
	Vector3 GetWorldPosition() { return _worldPosition; }
	Vector3 GetWorldRotation() { return _worldRotation; }
	Vector3 GetWorldScale() { return _worldScale; }

	void SetLocalPosition(const Vector3& position);
	void SetLocalRotation(const Vector3& rotation);
	void SetLocalScale(const Vector3& scale);
	void SetWorldPosition(const Vector3& position);
	void SetWorldRotation(const Vector3& rotation);
	void SetWorldScale(const Vector3& scale);

public:
	Matrix GetWorldMatrix() { return _world; }

private:
	void CalculateMatrix();
	void DecomposeMatrix();

public:
	bool HasParent() { return _parent != nullptr; }

	shared_ptr<Transform> GetParent() { return _parent; }
	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }

	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	Vector3 _localPosition = { 0.f, 0.f, 0.f };
	Vector3 _localRotation = { 0.f, 0.f, 0.f };
	Vector3 _localScale = { 1.f, 1.f, 1.f };
	Vector3 _worldPosition = { 0.f, 0.f, 0.f };
	Vector3 _worldRotation = { 0.f, 0.f, 0.f };
	Vector3 _worldScale = { 1.f, 1.f, 1.f };

	Matrix _local = ::XMMatrixIdentity();
	Matrix _world = ::XMMatrixIdentity();

	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};