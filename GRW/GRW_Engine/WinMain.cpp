#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wndClass = { 0 };

	const LPCWSTR pClassName = L"winApp";

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = DefWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = pClassName;
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);
	HWND hWnd = CreateWindowEx(
		0,
		pClassName,
		L"GRW Engine",
		(WS_VISIBLE | WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER),
		200, 200,
		1280, 720,
		nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);

	while (true)
	{

	}
	return 0;
}