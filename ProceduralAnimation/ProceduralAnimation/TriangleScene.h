#pragma once
#include<vector>

class Scene;
class Object;

class TriangleScene : public Scene
{
public:
	TriangleScene();
	~TriangleScene() override;

	void Init() override;
};