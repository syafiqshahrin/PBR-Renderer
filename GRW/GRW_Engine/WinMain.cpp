#include "Application.h"



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#pragma region OldCodeForRef

    /*
    Window AppWIndow(L"GRW_Engine", 1920, 1080, hInstance);

    //Dx11
    D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL FeatureLevelSupported;

    //Fill out swapchain description struct
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = AppWIndow.GetWidth();
    SwapChainDesc.BufferDesc.Height = AppWIndow.GetHeight();
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = AppWIndow.GetWindHandle();
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
    viewport.Width = AppWIndow.GetWidth();
    viewport.Height = AppWIndow.GetHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    gfxContext->RSSetViewports(1, &viewport);
    
    
    //load shader file onto gpu? Only needs to do this once on scene load
    //Bind shader to context - do this everytime we need to change shaders

    //Shader class - stores path, has shaderID for both vertex and pixel shader, dictionaries for shader data (one dictionary for each type)
    //LoadedShaderCollection Class - has a dictionary for both vertex and shader dx11 resource, key is path and resource is value

    //Vert Shader
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
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    gfxDevice->CreateInputLayout(inLayoutDesc, 2, vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), &inputLayoutHandle);
    gfxContext->IASetInputLayout(inputLayoutHandle.Get());

    //Create vertex buffer and Bind data to Input Assembler
    ////Dummy triangle mesh test data
    struct vertex
    {
        vertex(float x, float y, float z, float r, float g, float b)
        {
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;

            col[0] = r;
            col[1] = g;
            col[2] = b;
            //col[3] = a;
        }
        float pos[3] = {};
        float col[3] = {};
    };
    */

    /*
    vertex meshData[] = {
        vertex(-0.5f, -0.5f, -0.0f, 1,0,0),
        vertex(0.5f, -0.5f, -0.0f, 0,1,0),
        vertex(-0.5f, 0.5f, -0.0f, 0,0,1),
        vertex(0.5f, 0.5f, -0.0f, 1,1,0)
    };
    */
    /*
    vertex meshData[] = {
        vertex(-0.5f, -0.5f, -0.5f, 1,0,0), //0 - front bot left
        vertex(0.5f, -0.5f, -0.5f, 0,1,0), //1 - front bot right
        vertex(-0.5f, 0.5f, -0.5f, 0,0,1), //2 - front top left
        vertex(0.5f, 0.5f, -0.5f, 1,1,0), //3 - front top right
        vertex(-0.5f, -0.5f, 0.5f, 1,0,0), //4 - back bot left
        vertex(0.5f, -0.5f, 0.5f, 0,1,0), //5 - back bot right
        vertex(-0.5f, 0.5f, 0.5f, 0,0,1), //6 - back top left
        vertex(0.5f, 0.5f, 0.5f, 1,1,0) //7 - back top right
    };
    
    ////

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;

    D3D11_BUFFER_DESC verBufferDesc;
    verBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    verBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    verBufferDesc.ByteWidth = sizeof(meshData); //size of the whole array
    //verBufferDesc.StructureByteStride = sizeof(vertex); //stride of each vertex 
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
    */
        
    /*
    UINT meshIndicesData[]
    {
        0, 1, 2, 2, 1, 3
    };
    
    */
    /*
    UINT meshIndicesData[]
    {
        0, 1, 2, 
        1, 3, 2, 
        1, 5, 3, 
        5, 7, 3, 
        5, 4, 7, 
        4, 6, 7, 
        4, 2, 6, 
        0, 2, 6
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
    */

    /*
    //Set scissor rect
    //optional functionality
    //scissor is used to do additionally clipping within the viewport region
    D3D11_RECT scissorRect;
    scissorRect.left = 0 + (width/3) ;
    scissorRect.right = width - (width/3);
    scissorRect.top = 0 + (height / 3);
    scissorRect.bottom = height - (height / 3);
    
    //gfxContext->RSSetScissorRects(1, &scissorRect);
    */

    /*
    //Set Rasterizer State
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    //rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
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

    //////////

    //back buffer clear color
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //TEST
    Vector3 forward = Vector3(0, 0, 1);
    Vector3 right = Vector3(1, 0, 0);
    
    Vector3 added = forward + right;

    float ang = forward.Angle(added) * (180 / M_PI);
    //DEBUG("angle: " << ang);
    //DEBUG(added.prnt().c_str());
    //

    
    //Camera test
    Transform CamTes;
    CamTes.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    Transform cube;
    cube.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    cube.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    cube.SetScale(Vector3(1.2f, 1.2f, 1.2f));
    

    CamTes.UpdateMatrix();
    cube.UpdateMatrix();



    Matrix4x4 MVP = Matrix4x4::GetOrthoProjectionMatrix(Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWIndow.GetWidth(), AppWIndow.GetHeight())) * (CamTes.GetModelMatrix().GetInverse() * cube.GetModelMatrix());

    MVP = MVP.Transpose();

    ///cbuffer stuff
    Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;

    struct Cbuffer
    {
        float MVP[16];
    };

    Cbuffer cbuffer;
    MVP.GetMatrixFloatArray(cbuffer.MVP);

    for (int i = 0; i < 4; i++)
    {
        DEBUG(cbuffer.MVP[(i * 4)] << "," << cbuffer.MVP[(i * 4) + 1] << "," << cbuffer.MVP[(i * 4) + 2] << "," << cbuffer.MVP[(i * 4) + 3]);
    }

    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof(Cbuffer);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &cbuffer;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    hr = gfxDevice->CreateBuffer(&cbDesc, &data, &constBuffer);
    if (FAILED(hr))
    {
        DEBUG("Failed to create cbuffer");
    }

    gfxContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
    ///


    MSG msg = {};
    //App loop
    float deltaTime = 0;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        //windows messaging stuff
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        //render loop stuff
        gfxContext->ClearRenderTargetView(backBufferRenderView.Get(), clearColor);
        //draw triangle
        Vector3 rot = cube.GetRotation();
        rot.z += 5 * deltaTime;
        DEBUG("Angle = " << rot.z);
        cube.SetRotation(rot);
        cube.UpdateMatrix();
        Matrix4x4 MVP = Matrix4x4::GetOrthoProjectionMatrix(Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWIndow.GetWidth(), AppWIndow.GetHeight())) * (CamTes.GetModelMatrix().GetInverse() * cube.GetModelMatrix());
        MVP = MVP.Transpose();
        MVP.GetMatrixFloatArray(cbuffer.MVP);
        for (int i = 0; i < 4; i++)
        {
            DEBUG(cbuffer.MVP[(i * 4)] << "," << cbuffer.MVP[(i * 4) + 1] << "," << cbuffer.MVP[(i * 4) + 2] << "," << cbuffer.MVP[(i * 4) + 3]);
        }

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        gfxContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &cbuffer, sizeof(cbuffer));

        gfxContext->DrawIndexed(sizeof(meshIndicesData) / sizeof(UINT), 0, 0);

        gfxSwapChain->Present(0, 0); //flip buffers and vsync?
        std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count() / 1000.0f;
        DEBUG("Time difference = " << deltaTime );
    }
	return 0;
    */
#pragma endregion

    Application App(L"GRW_Engine", 1920, 1080, hInstance);
    App.StartApplication();
    return App.ApplicationUpdate();
}
 

