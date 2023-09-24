#include "Window.h"



Window::Window(LPCWSTR WinTitle, int w, int h, HINSTANCE hInstance)
{
	name = WinTitle;
	hInst = hInstance;
	width = w;
	height = h;
	InitialiseWinClass();
	InitialiseWindow();
}

Window::~Window()
{

}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

HWND Window::GetWindHandle()
{
	return hWnd;
}

int Window::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return -1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;

}



void Window::InitialiseWinClass()
{
	wndClass = { 0 };
	//Populate window struct 
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_OWNDC;
	//wndClass.lpfnWndProc = DefWindowProc;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.hInstance = hInst;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = pClassName;
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);
}

void Window::InitialiseWindow()
{
	hWnd = CreateWindowEx(
		0,
		pClassName,
		name,
		(WS_VISIBLE | WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER | WS_SYSMENU),
		200, 200,
		width, height,
		nullptr, nullptr, hInst, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wP, lP);
	}
}