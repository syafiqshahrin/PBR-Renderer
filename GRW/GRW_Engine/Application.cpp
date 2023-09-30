#include "Application.h"
#include "Window.h"
#include "Renderer.h"
#include <iostream>
#include <sstream>
#include "Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Debug.h"
#include "Transform.h"
#include "Matrix.h"
#include <chrono>
#include <comdef.h>
#include "Camera.h"
#include "json.hpp"
#include <fstream>
#include <string>
#include "GLTFMeshLoader.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "Texture.h"

Application::Application(LPCWSTR AppTitle, int w, int h, HINSTANCE hInstance)
{
	AppWindow = new Window(AppTitle, w, h, hInstance);
	AppRenderer = new Renderer(AppWindow->GetWindHandle(), w, h, true);
}

Application::~Application()
{
	delete AppWindow;
	delete AppRenderer;
}

void Application::StartApplication()
{
	//initialise resources and other stuff here
        //create and initialise transforms array (keeps track of all transforms)
        //create and initialise vertex shader array
        //create and initialise pixel shader array
        //    
        //Create meshes
            //Create vertex buffers
            //Create index buffers
        //Create Materials 
            //Create Shaders
                //Load shader
                    //Load shader from path and store in dx11 blob
                    //Create dx11 vertex shader object from blob
                    //Create Input layouts
                    //Create per material constant buffer 
        //Create Components
            //Create MeshRenderer component
                //Assign mesh and material    
        //Create GameObjects
            //Attach components
                //Attach MeshRenderer component

    
    //Gameplay Loop
    //Update All transforms matrix
    //Frustum culling
    //Sort mesh renderers into arrays by material 
    //Render Loop
        //Update per frame constant buffer (view matrix, projection matrix, time) 
        //Bind per frame constnat buffer (binding 0)
        //Opaque objects   
            //For each material
                    //Bind Vertex Buffer
                    //Bind Index Buffer
                    //Bind Vertex Shader
                    //Bind Input layout
                    //Bind Pixel Shader
                    //Update per material constant buffer (binding 1)
                    //Bind Constant Buffer
                    //For each object
                        //Calculate MVP
                        //Update per object constant buffer 
                        //Bind per object constant buffer (binding 2)
                        //Draw()

}

int Application::ApplicationUpdate()
{
    HRESULT hr;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(AppWindow->GetWindHandle());
    ImGui_ImplDX11_Init(AppRenderer->gfxDevice.Get(), AppRenderer->gfxContext.Get());
    
    //

    //Texture Loading
    Texture2D DiffuseTex("E:/My Documents/Assets/Substance Designer/Nyaowl Realm Rework/Nyaowl_Realm_Cliffs_basecolor.png");
    DiffuseTex.CreateTexture(AppRenderer);
    DiffuseTex.BindTexture(AppRenderer, 0);

    Texture2D NormalTex("E:/My Documents/Assets/Substance Designer/Nyaowl Realm Rework/Nyaowl_Realm_Cliffs_normal.png");
    NormalTex.CreateTexture(AppRenderer);
    NormalTex.BindTexture(AppRenderer, 1);
    

    //Sampler setup

    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = FLT_MAX;
    samplerDesc.MinLOD = -FLT_MAX;

    hr = AppRenderer->gfxDevice->CreateSamplerState(&samplerDesc, &SamplerState);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating sampler state - " << errorText);

        //return -1;
    }

    AppRenderer->gfxContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());

    //


    
    //Mesh loading

    //GLTFMeshLoader meshLoader("D:/Asset Files/Blender/FBX Files/Testgltf.gltf");
    GLTFMeshLoader meshLoader("E:/My Documents/Assets/Blender/FBX/TestGLTF.gltf");
    //GLTFMeshLoader meshLoader("E:/My Documents/Assets/Blender/FBX/SphereTest.gltf");

    std::vector<Vector3> posArray;
    meshLoader.GetVertexPositions(posArray);

    DEBUG("positions: " << posArray.size());

    /*
    for (Vector3 v : posArray)
    {
        DEBUG(v.prnt().c_str());
    }
    */

    std::vector<unsigned int> indArray;
    meshLoader.GetIndices(0, indArray);

    std::vector<Vector3> normArray;
    meshLoader.GetNormals(0, normArray);

    std::vector<Vector2> uvArray;
    meshLoader.GetUVs(0, uvArray);
    
    //

    



    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr <ID3DBlob> vshaderBlob;
    hr = D3DReadFileToBlob(L"../Shaders/BaseVertexShader.cso", &vshaderBlob);
    if (FAILED(hr))
    {
        std::cout << "Failed" << std::endl;
    }
    AppRenderer->gfxDevice->CreateVertexShader(vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), nullptr, &vertexShader);
    AppRenderer->gfxContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

    //Create and bind pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixShader;
    Microsoft::WRL::ComPtr <ID3DBlob> pshaderBlob;
    hr = D3DReadFileToBlob(L"../Shaders/BasePixelShader.cso", &pshaderBlob);
    AppRenderer->gfxDevice->CreatePixelShader(pshaderBlob->GetBufferPointer(), pshaderBlob->GetBufferSize(), nullptr, &pixShader);
    AppRenderer->gfxContext->PSSetShader(pixShader.Get(), nullptr, 0u);

    //Create input layout and bind it to the context
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayoutHandle;

    D3D11_INPUT_ELEMENT_DESC inLayoutDesc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    hr = AppRenderer->gfxDevice->CreateInputLayout(inLayoutDesc, 4, vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), &inputLayoutHandle);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creatiing input layout" << errorText);

        //return -1;
    }
    AppRenderer->gfxContext->IASetInputLayout(inputLayoutHandle.Get());

    //Create vertex buffer and Bind data to Input Assembler
    ////Dummy triangle mesh test data
    struct vertex
    {
        vertex(Vector3 p, Vector3 n, Vector2 uv, float r, float g, float b)
        {
            
            pos[0] = p.x;
            pos[1] = p.y;
            pos[2] = p.z;
            
            norm[0] = n.x;
            norm[1] = n.y;
            norm[2] = n.z;

            texc[0] = uv.x;
            texc[1] = uv.y;

            col[0] = r;
            col[1] = g;
            col[2] = b;
            //col[3] = a;
        }
        //Vector3 p;
        float pos[3] = {};
        float norm[3] = {};
        float texc[2] = {};
        float col[3] = {};
    };

    std::vector<vertex> meshData;
    for (int i = 0; i < posArray.size(); i++)
    {
        meshData.push_back(vertex(posArray[i], normArray[i], uvArray[i], 0, 1, 0));
    }
  
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
    */
    ////

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;

    D3D11_BUFFER_DESC verBufferDesc;
    verBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    verBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    verBufferDesc.ByteWidth = sizeof(vertex) * meshData.size(); //size of the whole array
    //verBufferDesc.StructureByteStride = sizeof(vertex); //stride of each vertex 
    verBufferDesc.CPUAccessFlags = 0u;
    verBufferDesc.MiscFlags = 0u;

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem = meshData.data();

    hr = AppRenderer->gfxDevice->CreateBuffer(&verBufferDesc, &srd, &vertBuffer);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creatiing vertex buffer" << errorText);

        //return -1;
    }


    const UINT stride = sizeof(vertex);
    const UINT offset = 0u;
    AppRenderer->gfxContext->IASetVertexBuffers(0u, 1u, vertBuffer.GetAddressOf(), &stride, &offset);


    //create index buffer and bind to input assembler
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;


    UINT meshIndicesData[]
    {
        0, 1, 2,
        1,3,2,
        1,5,3,
        5,7,3,
        5,4,7,
        4,6,7,
        4,0,6,
        0,2,6,
        2,3,6,
        3,7,6,
        4,5,0,
        5,1,0

    };

    D3D11_BUFFER_DESC indBufferDesc;
    indBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indBufferDesc.ByteWidth = sizeof(unsigned int) * indArray.size(); //size of the whole array
    //indBufferDesc.StructureByteStride = sizeof(UINT); //stride of each vertex 
    indBufferDesc.CPUAccessFlags = 0u;
    indBufferDesc.MiscFlags = 0u;

    srd.pSysMem = indArray.data();

    AppRenderer->gfxDevice->CreateBuffer(&indBufferDesc, &srd, &indexBuffer);
    AppRenderer->gfxContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //set primitive topology type
    AppRenderer->gfxContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /*
    //Set scissor rect
    //optional functionality
    //scissor is used to do additionally clipping within the viewport region
    D3D11_RECT scissorRect;
    scissorRect.left = 0 + (width/3) ;
    scissorRect.right = width - (width/3);
    scissorRect.top = 0 + (height / 3);
    scissorRect.bottom = height - (height / 3);

    //AppRenderer->gfxContext->RSSetScissorRects(1, &scissorRect);
    */



    //////////

    
    //Camera test
    Transform CamTes;
    CamTes.SetPosition(Vector3(0.0f,0.0f, 0.0f));
    CamTes.SetRotation(Vector3(0.0f,0.0f, 0.0f));
    OrthoCamera cam(CamTes, Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWindow->GetWidth(), AppWindow->GetHeight()));
    PerspectiveCamera pCam(CamTes, 60, 0.1f, 100000.0f, Vector2(AppWindow->GetWidth(), AppWindow->GetHeight()));
    

    Transform cube;
    cube.SetPosition(Vector3(0.0f, 0.0f, 20.0f));
    cube.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    cube.SetScale(Vector3(1.f, 1.f, 1.f));
    cube.UpdateMatrix();

    Matrix4x4 MVP = pCam.GetCameraProjectionMatrix() * (pCam.GetCameraViewMatrix() * cube.GetModelMatrix());

    MVP = MVP.Transpose();

    Vector3 lightDirNorm = Vector3::zero();
    lightDirNorm.y = -1.0f;

    ///cbuffer stuff
    Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;

    struct Cbuffer
    {
        Vector4 time;
        float MVP[16];
        float MW[16];
        float MC[16];
        float MNorm[16];
        Vector4 light;
    };

    Cbuffer cbuffer;
    cbuffer.time.x = 0;
    cbuffer.light = lightDirNorm.GetVec4(false);

    Matrix4x4 MWorld = cube.GetModelMatrix();
    Matrix4x4 MNormal = MWorld.GetMat3x3().GetInverse().GetMat4x4();
    
    Matrix4x4 MView = CamTes.GetModelMatrix();
    
    MWorld.Transpose();
    MView.Transpose();
    
    MVP.GetMatrixFloatArray(cbuffer.MVP);
    MWorld.GetMatrixFloatArray(cbuffer.MW);
    MView.GetMatrixFloatArray(cbuffer.MC);
    MNormal.GetMatrixFloatArray(cbuffer.MNorm);


    /*
    for (int i = 0; i < 4; i++)
    {
        DEBUG(cbuffer.MVP[(i * 4)] << "," << cbuffer.MVP[(i * 4) + 1] << "," << cbuffer.MVP[(i * 4) + 2] << "," << cbuffer.MVP[(i * 4) + 3]);
    }
    */

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

    hr = AppRenderer->gfxDevice->CreateBuffer(&cbDesc, &data, &constBuffer);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create cbuffer:" << errorText);
    }

    AppRenderer->gfxContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
    AppRenderer->gfxContext->PSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

    //




    //App loop
    float deltaTime = 0;
    while (true)
    {
        std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        if (AppWindow->ProcessMessages() == -1)
        {
            break;
        }
        //render loop stuff
        //draw triangle
        Vector3 rot = cube.GetRotation();
        //rot.z += 0.1f * deltaTime;
        //rot.x += 0.2f * deltaTime;
        rot.y += 0.1f * deltaTime;
        rot.y = fmod(rot.y, 360.0f);

        //updating cbuffer struct

        cube.SetRotation(rot);
        cube.UpdateMatrix();
        CamTes.UpdateMatrix();
        
        MVP = pCam.GetCameraProjectionMatrix() * (pCam.GetCameraViewMatrix() * cube.GetModelMatrix());
        MVP = MVP.Transpose();
        MVP.GetMatrixFloatArray(cbuffer.MVP);

        MWorld = cube.GetModelMatrix();
        MNormal = MWorld.GetMat3x3().GetInverse().GetMat4x4();
        MView = CamTes.GetModelMatrix().Transpose();
        
        MWorld = MWorld.Transpose();
        MWorld.GetMatrixFloatArray(cbuffer.MW);
        MView.GetMatrixFloatArray(cbuffer.MC);
        MNormal.GetMatrixFloatArray(cbuffer.MNorm);


        cbuffer.time.x += deltaTime;
        cbuffer.light = lightDirNorm.GetVec4(false);


        //update constant buffer data on gpu 
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        AppRenderer->gfxContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &cbuffer, sizeof(cbuffer));
        AppRenderer->gfxContext->Unmap(constBuffer.Get(), 0);

        AppRenderer->ClearBackbuffer();
        AppRenderer->gfxContext->DrawIndexed(indArray.size() , 0, 0);

        //ImGui Stuff

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            static float f = 0.0f;
            Vector3 pos = cube.GetPosition();
            Vector3 rot = cube.GetRotation();
            Vector3 scale = cube.GetScale();
            float* p[3] = { &pos.x, &pos.y, &pos.z };
            float* r[3] = { &rot.x, &rot.y, &rot.z };
            float* s[3] = { &scale.x, &scale.y, &scale.z };

            Vector3 posC = CamTes.GetPosition();
            Vector3 rotC = CamTes.GetRotation();

            float* pC[3] = { &posC.x, &posC.y, &posC.z };
            float* rC[3] = { &rotC.x, &rotC.y, &rotC.z };

            float* lD[3] = { &lightDirNorm.x, &lightDirNorm.y, &lightDirNorm.z };

            ImGui::Begin("Transforms");                         


            ImGui::Text("Cube Transform:");
            ImGui::SliderFloat3("Position", *p, -100, 100);            
            ImGui::SliderFloat3("Rotation", *r, -100, 100);           
            ImGui::SliderFloat3("Scale", *s, -100, 100);            
            cube.SetPosition(pos);
            cube.SetRotation(rot);
            cube.SetScale(scale);
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            
            ImGui::Text("Camera Transform:");
            ImGui::SliderFloat3("Cam Position", *pC, -100, 100);
            ImGui::SliderFloat3("Cam Rotation", *rC, -100, 100);
            CamTes.SetPosition(posC);
            CamTes.SetRotation(rotC);

            ImGui::Text("Light:");
            ImGui::SliderFloat3("Direction", *lD, -1, 1);
            
            //ImGui::SameLine();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        AppRenderer->UpdateSwapchain();


        std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count() / 1000.0f;
        //DEBUG("Time difference = " << deltaTime);
    }
    return 0;
}
