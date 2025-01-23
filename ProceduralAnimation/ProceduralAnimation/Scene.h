#pragma once

class Object;

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

public:
	virtual void Init() = 0;
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(shared_ptr<Renderer> renderer);

public:
	vector<shared_ptr<Object>>& GetObjects() { return _objects; }
	
protected:
	shared_ptr<Camera> _camera;
	vector<shared_ptr<Object>> _objects;
};