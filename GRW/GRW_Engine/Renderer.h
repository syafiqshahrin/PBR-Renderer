#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

enum class PrimType {TRIANGLE_LIST, LINE};

class Renderer
{
public:
	Renderer(HWND hWnd, int WindowWidth, int WindowHeight, bool Windowed);
	bool InitializeRenderer(HWND hWnd, int WindowWidth, int WindowHeight, bool Windowed);
	bool SetWindowed(bool Windowed);
	void SetPrimitiveRenderingMode(const PrimType primitive);

	bool BindVertexBuffer();
	bool BindIndexBuffer();
	bool BindShader();

	Microsoft::WRL::ComPtr<ID3D11Device> gfxDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> gfxContext;

	void ClearBackbuffer();
	void UpdateSwapchain();
	void BindBackBufferAsRenderTarget();
	void SetViewport(int w, int h);
	void RestoreViewport();

private:
	HRESULT hr;
	
	//Main d3d11 handles
	Microsoft::WRL::ComPtr<IDXGISwapChain> gfxSwapChain;

	//Swapchain stuff
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTexResource;

	//Rasterizer and viewport stuff
	D3D11_VIEWPORT BaseViewport;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	D3D11_RASTERIZER_DESC rasterizerDesc;

	//Depth Stencil Buffer stuff
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilTexResource;
	D3D11_TEXTURE2D_DESC DepthStencilTexDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

private:
	bool InitializeDepthStencilBuffer();
	bool SwapChainAndDepthBufferInitialised;

};