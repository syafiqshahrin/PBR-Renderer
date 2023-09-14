#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>

//windows callback function for handling messages from OS
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
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


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //create window class struct
	WNDCLASSEX wndClass = { 0 };

    //window class name
	const LPCWSTR pClassName = L"winApp";

    float width = 1920;
    float height = 1080;

    //Populate window struct 
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_OWNDC;
	//wndClass.lpfnWndProc = DefWindowProc;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = pClassName;
	wndClass.hIconSm = nullptr;

    //Register window class
	RegisterClassEx(&wndClass);
    //Create window and get the handle to window
	HWND hWnd = CreateWindowEx(
		0,
		pClassName,
		L"GRW Engine",
		(WS_VISIBLE | WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER | WS_SYSMENU),
		200, 200,
        1920, 1080,
		nullptr, nullptr, hInstance, nullptr
	);

    //Show window
	ShowWindow(hWnd, SW_SHOW);


    //Dx11
    
    D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL FeatureLevelSupported;

    //Fill out swapchain description struct
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = width;
    SwapChainDesc.BufferDesc.Height = height;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = hWnd;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality - 1;
    SwapChainDesc.Windowed = TRUE;

    //Smart Pointers that will auto release the com handles (would have to do it manually if using normal pointers)
    Microsoft::WRL::ComPtr<ID3D11Device> gfxDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> gfxContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> gfxSwapChain;

    //Create DX11 Device - Create ID3D11Device object and call D3D11CreateDeviceAndSwapChain 
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &gfxSwapChain, &gfxDevice, &FeatureLevelSupported, &gfxContext);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize dx11 deve/swapchain" << std::endl;
    }

    //get the back buffer from the swap chain and store it in a tex2d handle
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferResource;
    hr = gfxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferResource);
    //set back buffer handle as the render target
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferRenderView;
    hr = gfxDevice->CreateRenderTargetView(backBufferResource.Get(), nullptr , &backBufferRenderView);
    gfxContext->OMSetRenderTargets(1, backBufferRenderView.GetAddressOf(), nullptr);
    //create viewport and set view port
    D3D11_VIEWPORT viewport;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    gfxContext->RSSetViewports(1, &viewport);
    
    //Create and bind Vertex Shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr <ID3DBlob> vshaderBlob;
    hr = D3DReadFileToBlob(L"../Shaders/BaseVertexShader.cso", &vshaderBlob);
    if (FAILED(hr))
    {
        std::cout << "Failed" << std::endl;
    }
    gfxDevice->CreateVertexShader(vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), nullptr, &vertexShader);
    gfxContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

    //Create and bind pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixShader;
    Microsoft::WRL::ComPtr <ID3DBlob> pshaderBlob;
    hr = D3DReadFileToBlob(L"../Shaders/BasePixelShader.cso", &pshaderBlob);
    gfxDevice->CreatePixelShader(pshaderBlob->GetBufferPointer(), pshaderBlob->GetBufferSize(), nullptr, &pixShader);
    gfxContext->PSSetShader(pixShader.Get(), nullptr, 0u);
    
    //Create input layout and bind it to the context
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayoutHandle;

    D3D11_INPUT_ELEMENT_DESC inLayoutDesc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    gfxDevice->CreateInputLayout(inLayoutDesc, 1u, vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), &inputLayoutHandle);
    gfxContext->IASetInputLayout(inputLayoutHandle.Get());

    //Create vertex buffer and Bind data to Input Assembler
    ////Dummy triangle mesh test data
    struct vertex
    {
        vertex(float x, float y, float z)
        {
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
        }
        float pos[3] = {};
    };

    vertex meshData[] = {
        {vertex(-0.5f, -0.5f, 0.0f)},
        {vertex(0.0f, 0.5f, 0.0f)},
        {vertex(0.5f, -0.5f, 0.0f)}
    };
    ////

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;

    D3D11_BUFFER_DESC verBufferDesc;
    verBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    verBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    verBufferDesc.ByteWidth = sizeof(meshData); //size of the whole array
    verBufferDesc.StructureByteStride = sizeof(vertex); //stride of each vertex 
    verBufferDesc.CPUAccessFlags = 0u;
    verBufferDesc.MiscFlags = 0u;

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem = meshData;

    hr = gfxDevice->CreateBuffer(&verBufferDesc, &srd, &vertBuffer);
    if (FAILED(hr))
    {
        std::cout << "Failed creatiing buffer" << std::endl;

        return -1;
    }


    const UINT stride = sizeof(vertex);
    const UINT offset = 0u;
    gfxContext->IASetVertexBuffers(0u, 1u, vertBuffer.GetAddressOf(), &stride, &offset);


    //create index buffer and bind to input assembler
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    UINT meshIndicesData[]
    {
        0, 1, 2
    };

    D3D11_BUFFER_DESC indBufferDesc;
    indBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indBufferDesc.ByteWidth = sizeof(meshIndicesData); //size of the whole array
    indBufferDesc.StructureByteStride = sizeof(UINT); //stride of each vertex 
    indBufferDesc.CPUAccessFlags = 0u;
    indBufferDesc.MiscFlags = 0u;

    srd.pSysMem = meshIndicesData;

    gfxDevice->CreateBuffer(&indBufferDesc, &srd, &indexBuffer);
    gfxContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //set primitive topology type
    gfxContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Set scissor rect
    //optional functionality
    //scissor is used to do additionally clipping within the viewport region
    D3D11_RECT scissorRect;
    scissorRect.left = 0 + (width/3) ;
    scissorRect.right = width - (width/3);
    scissorRect.top = 0 + (height / 3);
    scissorRect.bottom = height - (height / 3);

    gfxContext->RSSetScissorRects(1, &scissorRect);

    //Set Rasterizer State
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    rasterizerDesc.FrontCounterClockwise = true;
    rasterizerDesc.DepthBias = false;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = true;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    
    gfxDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    gfxContext->RSSetState(rasterizerState.Get());

    //////////

    //back buffer clear color
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    MSG msg = {};
    //App loop
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        //windows messaging stuff
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        //render loop stuff
        gfxContext->ClearRenderTargetView(backBufferRenderView.Get(), clearColor);
        //draw triangle
        gfxContext->DrawIndexed(sizeof(meshIndicesData) / sizeof(UINT), 0, 0);

        gfxSwapChain->Present(0, 0); //flip buffers and vsync?
    }

	return 0;
}

#pragma region simpleD311AppRef
/*
#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;

HRESULT CompileShader(const std::string& shader, const std::string& target, ComPtr<ID3DBlob>& code_blob)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> error_blob = nullptr;

    HRESULT hr = D3DCompile(shader.c_str(), shader.length(), "none", nullptr, nullptr, "main", target.c_str(), shader_flags, 0, code_blob.GetAddressOf(), error_blob.GetAddressOf());
    if (FAILED(hr)) {
        if (error_blob != nullptr) {
            printf("Failed to compile shader: %s\n", (char*)error_blob->GetBufferPointer());
        }

        return hr;
    }

    return S_OK;
}

HRESULT Initialize(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& context)
{
    // vertex shader

    std::string vs_source =
        "float4 main(in float4 pos : POSITION) : SV_Position\n"
        "{\n"
        "    return pos;\n"
        "}\n";

    ComPtr<ID3DBlob> vs_blob = nullptr;
    HRESULT hr = CompileShader(vs_source, "vs_5_0", vs_blob);
    if (FAILED(hr)) {
        printf("Failed to compile vertex shader.\n");
        return hr;
    }

    ComPtr<ID3D11VertexShader> vs = nullptr;
    hr = device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vs);
    if (FAILED(hr)) {
        return hr;
    }

    context->VSSetShader(vs.Get(), nullptr, 0);

    // input layout

    D3D11_INPUT_ELEMENT_DESC position = {};
    position.SemanticName = "POSITION";
    position.Format = DXGI_FORMAT_R32G32_FLOAT;
    position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    ComPtr<ID3D11InputLayout> il = nullptr;
    hr = device->CreateInputLayout(&position, 1, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &il);
    if (FAILED(hr)) {
        return hr;
    }

    context->IASetInputLayout(il.Get());

    // pixel shader

    std::string ps_source =
        "float4 main(in float4 screenPos : SV_Position) : SV_Target\n"
        "{\n"
        "    return float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n";

    ComPtr<ID3DBlob> ps_blob = nullptr;
    hr = CompileShader(ps_source, "ps_5_0", ps_blob);
    if (FAILED(hr)) {
        printf("Failed to compile fragment shader.\n");
        return hr;
    }

    ComPtr<ID3D11PixelShader> ps = nullptr;
    hr = device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &ps);
    if (FAILED(hr)) {
        return hr;
    }

    context->PSSetShader(ps.Get(), nullptr, 0);

    // vertex buffer

    float vertices[] = {
        -0.9f,-0.9f,
        -0.9f, 0.9f,
         0.9f,-0.9f,
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA srd = {};
    srd.pSysMem = vertices;

    ComPtr<ID3D11Buffer> vb;
    hr = device->CreateBuffer(&bd, &srd, &vb);
    if (FAILED(hr)) {
        return hr;
    }

    UINT stride = sizeof(float) * 2;
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);

    // index buffer

    unsigned short indices[] = {
        0, 1, 2,
    };

    bd.ByteWidth = sizeof(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    srd.pSysMem = indices;

    ComPtr<ID3D11Buffer> ib;
    hr = device->CreateBuffer(&bd, &srd, &ib);
    if (FAILED(hr)) {
        return hr;
    }

    context->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R16_UINT, 0);

    // primitive topology

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return S_OK;
}

void Render(const ComPtr<ID3D11DeviceContext>& context, const ComPtr<ID3D11RenderTargetView>& back_buffer)
{
    float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    context->ClearRenderTargetView(back_buffer.Get(), green);

    context->DrawIndexed(3, 0, 0);
}

int main()
{
    int client_width = 256;
    int client_height = 256;

    // create a window

    std::wstring window_class = L"WindowClass";

    WNDCLASS wc = {};
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = window_class.c_str();
    wc.lpfnWndProc = [](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (message == WM_CLOSE || (message == WM_KEYDOWN && wparam == VK_ESCAPE)) {
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, message, wparam, lparam);
    };


    RegisterClass(&wc);

    DWORD dwStyle = WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;

    RECT rc = { 0, 0, client_width, client_height };
    AdjustWindowRect(&rc, dwStyle, FALSE);

    HWND hwnd = CreateWindow(window_class.c_str(), L"Direct3D 11", dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);
    if (hwnd == NULL) {
        printf("Failed to create window...\n");
        return 1;
    }

    // initialize Direct3D

    UINT create_device_flags = 0;
#ifdef _DEBUG
    create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<ID3D11Device> device = nullptr;
    ComPtr<ID3D11DeviceContext> context = nullptr;
    ComPtr<IDXGISwapChain> swap_chain = nullptr;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, create_device_flags, nullptr, 0, D3D11_SDK_VERSION, &scd, &swap_chain, &device, nullptr, &context);
    if (FAILED(hr)) {
        printf("Failed to initialize Direct3D...\n");
        return 1;
    }

    ComPtr<ID3D11Texture2D> buffer = nullptr;
    hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&buffer));
    if (FAILED(hr)) {
        printf("Failed to initialize Direct3D...\n");
        return 1;
    }

    ComPtr<ID3D11RenderTargetView> back_buffer = nullptr;
    hr = device->CreateRenderTargetView(buffer.Get(), nullptr, &back_buffer);
    if (FAILED(hr)) {
        printf("Failed to initialize Direct3D...\n");
        return 1;
    }

    context->OMSetRenderTargets(1, back_buffer.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.0f, 0.0f, (float)client_width, (float)client_height);
    context->RSSetViewports(1, &vp);

    // initialize rendering

    hr = Initialize(device, context);
    if (FAILED(hr)) {
        printf("Failed to initialize rendering...\n");
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Render(context, back_buffer);
            swap_chain->Present(0, 0);
        }
    }

    DestroyWindow(hwnd);
    UnregisterClass(window_class.c_str(), NULL);

    return 0;
}
*/
#pragma endregion

