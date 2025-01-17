#pragma once

class Object;

class Scene
{
public:
	virtual ~Scene() {}
	virtual void Init() = 0;
	void Update();
	void Render();

protected:
	vector<Object*> objects;
};