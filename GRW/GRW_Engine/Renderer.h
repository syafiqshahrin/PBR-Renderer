#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>


class Renderer
{
public:
	Renderer(HWND hWnd, float WindowWidth, float WindowHeight, bool Windowed);
	bool InitializeRenderer(HWND hWnd, float WindowWidth, float WindowHeight, bool Windowed);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> gfxDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> gfxContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> gfxSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	D3D11_VIEWPORT *BaseViewport;

};