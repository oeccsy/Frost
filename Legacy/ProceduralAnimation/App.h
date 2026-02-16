#pragma once

class Scene;

class App
{
public:
	App();
	~App();

public:
	void Init(HWND hwnd);
	void Run();

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Renderer> _renderer;

	shared_ptr<Scene> _currentScene;
};