#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Renderer;
class Window;

class Application
{
public:
	Application(LPCWSTR AppTitle, int w, int h, HINSTANCE hInstance);
	~Application();

	void StartApplication();
	int ApplicationUpdate();
private:
	void InitialiseApplication();
	Window* AppWindow;
	Renderer* AppRenderer;
};