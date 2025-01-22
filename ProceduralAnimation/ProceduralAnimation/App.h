#pragma once

class App
{
public:
	App();
	~App();

public:
	void Init(HWND hwnd);
	int Run();
	void LoadScene(class Scene* newScene);

	void CreateConsole();

private:
	HWND _hwnd;
	class Scene* currentScene;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;
};