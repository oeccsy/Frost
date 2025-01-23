#pragma once
#include<vector>

class Scene;
class Object;

class TriangleScene : public Scene
{
public:
	TriangleScene();
	~TriangleScene() override;

public:
	void Init() override;
};