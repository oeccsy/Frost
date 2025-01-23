#pragma once

class Object;

class Scene
{
public:
	virtual ~Scene() {}
	virtual void Init() = 0;
	vector<shared_ptr<Object>>& GetObjects() { return _objects; }
	
protected:
	shared_ptr<Camera> _camera;
	vector<shared_ptr<Object>> _objects;
};