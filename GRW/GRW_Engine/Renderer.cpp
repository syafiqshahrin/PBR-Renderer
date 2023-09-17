#include <Windows.h> 
#include "Renderer.h"
#include <iostream>

Renderer::Renderer(HWND hWnd, float WindowWidth, float WindowHeight, bool Windowed)
{
	
}

bool Renderer::InitializeRenderer(HWND hWnd, float WindowWidth, float WindowHeight, bool Windowed)
{
    D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL FeatureLevelSupported;

    //Fill out swapchain description struct
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = WindowWidth;
    SwapChainDesc.BufferDesc.Height = WindowHeight;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = hWnd;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality - 1;
    SwapChainDesc.Windowed = Windowed;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &gfxSwapChain, &gfxDevice, &FeatureLevelSupported, &gfxContext);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize dx11 deve/swapchain" << std::endl;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferResource;
    hr = gfxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferResource);
    //Create target view from tex2d
    hr = gfxDevice->CreateRenderTargetView(backBufferResource.Get(), nullptr, &BackBufferRTV);
    if (FAILED(hr))
    {
        std::cout << "Failed to bind back buffer as render target" << std::endl;
    }
    //bind render target
    gfxContext->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), nullptr);

    BaseViewport->Width = WindowWidth;
    BaseViewport->Height = WindowHeight;
    BaseViewport->MinDepth = 0.0f;
    BaseViewport->MaxDepth = 1.0f;
    BaseViewport->TopLeftX = 0.0f;
    BaseViewport->TopLeftY = 0.0f;

    gfxContext->RSSetViewports(1, BaseViewport);

	return false;
}
