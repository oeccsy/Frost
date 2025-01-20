#pragma once

class Object;

class Scene
{
public:
	virtual ~Scene() {}
	virtual void Init() = 0;
	vector<shared_ptr<Object>>& GetObjects() { return objects; }
	
protected:
	vector<shared_ptr<Object>> objects;
};