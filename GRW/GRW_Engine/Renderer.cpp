#include <Windows.h> 
#include "Renderer.h"
#include <iostream>
#include <comdef.h>
#include "Debug.h"

Renderer::Renderer(HWND hWnd, int WindowWidth, int WindowHeight, bool Windowed)
{
    SwapChainAndDepthBufferInitialised = false;
    InitializeRenderer(hWnd, WindowWidth, WindowHeight, Windowed);
}

bool Renderer::InitializeRenderer(HWND hWnd, int WindowWidth, int WindowHeight, bool Windowed)
{
    D3D_FEATURE_LEVEL FeatureLevel[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL FeatureLevelSupported;

    //Fill out swapchain description struct

    //ZeroMemory(SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = WindowWidth;
    SwapChainDesc.BufferDesc.Height = WindowHeight;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = hWnd;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.Windowed = Windowed;

    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &gfxSwapChain, &gfxDevice, &FeatureLevelSupported, &gfxContext);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create graphics device and swapchain:" << errorText);
        return false;
    }

    //Create target view from swap chain back buffer
    hr = gfxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexResource);
    hr = gfxDevice->CreateRenderTargetView(backBufferTexResource.Get(), nullptr, &BackBufferRTV);
    if (FAILED(hr))
    {
        std::cout << "Failed to bind back buffer as render target" << std::endl;
        return false;
    }

    //set and bind viewport
    BaseViewport.Width = WindowWidth;
    BaseViewport.Height = WindowHeight;
    BaseViewport.MinDepth = 0.0f;
    BaseViewport.MaxDepth = 1.0f;
    BaseViewport.TopLeftX = 0.0f;
    BaseViewport.TopLeftY = 0.0f;

    gfxContext->RSSetViewports(1, &BaseViewport);


    //Set and bind Rasterizer State
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = true;
    rasterizerDesc.DepthBias = false;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;

    gfxDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    
    gfxContext->RSSetState(rasterizerState.Get());


    //
    InitializeDepthStencilBuffer();
    SwapChainAndDepthBufferInitialised = true;
    //bind render target
    gfxContext->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), DepthStencilView.Get());
	return true;
}

bool Renderer::SetWindowed(bool Windowed)
{

    hr = gfxSwapChain->SetFullscreenState(!Windowed, nullptr);
  
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

void Renderer::SetPrimitiveRenderingMode(const PrimType primitive)
{
    D3D_PRIMITIVE_TOPOLOGY topology;

    switch (primitive)
    {
        case PrimType::TRIANGLE_LIST:
            topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case PrimType::LINE:
            topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        default:
            topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
    }

    gfxContext->IASetPrimitiveTopology(topology);
}

void Renderer::ClearBackbuffer()
{
    float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    gfxContext->ClearRenderTargetView(BackBufferRTV.Get(), clearColor);
    gfxContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::UpdateSwapchain()
{
    gfxSwapChain->Present(0,0);
}

void Renderer::BindBackBufferAsRenderTarget()
{
    if(SwapChainAndDepthBufferInitialised)
        gfxContext->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), DepthStencilView.Get());
}

void Renderer::SetViewport(int w, int h)
{
    BaseViewport.Width = w;
    BaseViewport.Height = h;
    BaseViewport.MinDepth = 0.0f;
    BaseViewport.MaxDepth = 1.0f;
    BaseViewport.TopLeftX = 0.0f;
    BaseViewport.TopLeftY = 0.0f;

    gfxContext->RSSetViewports(1, &BaseViewport);
}

void Renderer::UpdateRasterizerState()
{
    gfxDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    gfxContext->RSSetState(rasterizerState.Get());
}





bool Renderer::InitializeDepthStencilBuffer()
{
    //Fill tex2d desc struct
    D3D11_TEXTURE2D_DESC backbufferDesc;
    backBufferTexResource->GetDesc(&backbufferDesc);
    DepthStencilTexDesc.Width = backbufferDesc.Width;
    DepthStencilTexDesc.Height = backbufferDesc.Height;
    DepthStencilTexDesc.MipLevels = 1;
    DepthStencilTexDesc.ArraySize = 1;
    DepthStencilTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    DepthStencilTexDesc.SampleDesc.Count = 1;
    DepthStencilTexDesc.SampleDesc.Quality = 0;
    DepthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; //for when we want to use the depth texture in a shader
    DepthStencilTexDesc.CPUAccessFlags = 0;
    DepthStencilTexDesc.MiscFlags = 0;
    //create texture2D resource device->CreateTexture2D()
    hr = gfxDevice->CreateTexture2D(&DepthStencilTexDesc, nullptr, &DepthStencilTexResource);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create depth render target view:" << errorText);
        return false;
    }
    //Fill depth stencil desc struct
    DepthStencilDesc.DepthEnable = true;
    DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
                    
    DepthStencilDesc.StencilEnable = true;
    DepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    DepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
                    
    DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                    
    DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    //Create depth stencil state resource device->CreateDepthStencilState()
    hr = gfxDevice->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create depth state:" << errorText);
        return false;
    }
    //Bind depth stencil state device->OMSetDepthStencilState()
    gfxContext->OMSetDepthStencilState(DepthStencilState.Get(), 1);
    //Fill depth stencil view desc struct
    DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DepthStencilViewDesc.Texture2D.MipSlice = 0;
    DepthStencilViewDesc.Flags = 0;

    //create depth stencil view resource device->CreateDepthStencilView()
    hr = gfxDevice->CreateDepthStencilView(DepthStencilTexResource.Get(), &DepthStencilViewDesc, &DepthStencilView);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create depth buffer:" << errorText);
        return false;
    }
    //Bind depth stencil view with context->OMSetRenderTargets


    return true;
}
