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
#include "Light.h"
#include "Mesh.h"
#include "CBuffer.h"
#include "Shader.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include <filesystem>

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
    //Test listing filepaths in directory
    std::string path = "../Assets/Textures";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        DEBUG(entry.path());
    }
    HRESULT hr;



#pragma region IMGUI Setup
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
#pragma endregion


#pragma region Texture Loading and Setup
    //Texture Loading
    //Texture2D DiffuseTex("E:/My Documents/Assets/Substance Designer/Materials/Wood/Wood_basecolor.png");
    //Texture2D DiffuseTex("E:/My Documents/Assets/Substance Designer/Homestead Realm/Homestead_Cliff_Mat__Warmer_Higher_Detail_basecolor.png");
    //Texture2D DiffuseTex("C:/Users/syafiq.shahrin/Downloads/resting_place_2_2k.hdr");
    //Texture2D DiffuseTex("E:/My Documents/Downloads/little_paris_eiffel_tower_2k.hdr");
    //Texture2D DiffuseTex("../Assets/Textures/TexturedSurface_basecolor.png");
    Texture2D DiffuseTex("../Assets/Textures/TexturedSurface2_basecolor.png");
    //Texture2D DiffuseTex("D:/Asset Files/Substance Designer/Misc/TexturedSurface_basecolor.png");
    DiffuseTex.CreateTextureFromFile(AppRenderer, 8, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
    DiffuseTex.BindTexture(AppRenderer, 0);


    //Texture2D NormalTex("E:/My Documents/Assets/Substance Designer/Materials/Wood/Wood_normal.png");
    //Texture2D NormalTex("E:/My Documents/Assets/Substance Designer/Homestead Realm/Homestead_Cliff_Mat__Warmer_Higher_Detail_normal.png");
    //Texture2D NormalTex("D:/Asset Files/Substance Designer/Misc/TexturedSurface_normal.png");
    //Texture2D NormalTex("../Assets/Textures/TexturedSurface_normal.png");
    Texture2D NormalTex("../Assets/Textures/TexturedSurface2_normal.png");
    NormalTex.CreateTextureFromFile(AppRenderer);
    NormalTex.BindTexture(AppRenderer, 1);

    //Texture2D RMATex("../Assets/Textures/TexturedSurface_RMA.png");
    Texture2D RMATex("../Assets/Textures/TexturedSurface2_RMA.png");
    //Texture2D RMATex("D:/Asset Files/Substance Designer/Misc/TexturedSurface_RMA.png");
    //Texture2D RMATex("E:/My Documents/Assets/Substance Designer/Materials/Wood/Wood_RMA.png");
    //Texture2D RMATex("E:/My Documents/Assets/Substance Designer/Homestead Realm/Homestead_Cliff_Mat__Warmer_Higher_Detail_RMA.png");
    RMATex.CreateTextureFromFile(AppRenderer);
    RMATex.BindTexture(AppRenderer, 2);

    //Texture2D HDRI("E:/My Documents/Downloads/newport_loft.hdr",4, true);
    //Texture2D HDRI("E:/My Documents/Downloads/chinese_garden_2k.hdr",4, true);
    //Texture2D HDRI("E:/My Documents/Downloads/little_paris_eiffel_tower_2k.hdr",4, true);
    Texture2D HDRI("../Assets/Textures/resting_place_2_2k.hdr", 4, true);
    DEBUG("HDRI");
    HDRI.CreateTextureFromFile(AppRenderer, 8, false);

#pragma endregion

#pragma region Sampler Setup temp
    //Sampler setup
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = FLT_MAX;
    samplerDesc.MinLOD = 0.0f;

    hr = AppRenderer->gfxDevice->CreateSamplerState(&samplerDesc, &SamplerState);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating sampler state - " << errorText);

        //return -1;
    }

    AppRenderer->gfxContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());
#pragma endregion

#pragma region Mesh Loading and mesh binding


    Mesh sphereMesh("../Assets/Mesh/SphereTest.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/RoundedCylinder.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/SphereTest.gltf");
    //Mesh Skybox("E:/My Documents/Assets/Blender/FBX/RoundedCylinder.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/CubicCylinder.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/Donut.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/BarrelTest.gltf");
    //Mesh sphereMesh("E:/My Documents/Assets/Blender/FBX/UnitCube.gltf");
    sphereMesh.CreateMeshFromFile(AppRenderer);
    
    //Mesh Skybox("E:/My Documents/Assets/Blender/FBX/SphereTest.gltf");
    Mesh Skybox("../Assets/Mesh/SphereTest.gltf");
    Skybox.CreateMeshFromFile(AppRenderer);

    //
#pragma endregion

#pragma region Vertex and Pixel Shader setup temp

    VertexShader baseVertShader("../Shaders/BaseVertexShader.cso");
    baseVertShader.CreateShader(AppRenderer);
    PixelShader basePixShader("../Shaders/BasePixelShader.cso");
    basePixShader.CreateShader(AppRenderer);

    VertexShader SkyboxVertShader("../Shaders/SkyboxVertShader.cso");
    SkyboxVertShader.CreateShader(AppRenderer);
    PixelShader SkyboxPixShader("../Shaders/SkyboxPixShader.cso");
    SkyboxPixShader.CreateShader(AppRenderer);

    VertexShader hdrVertShader("../Shaders/PrefilterCubeMapVertShader.cso");
    hdrVertShader.CreateShader(AppRenderer);
    PixelShader hdrPixShader("../Shaders/PrefilterCubeMapPixShader.cso");
    hdrPixShader.CreateShader(AppRenderer);

    VertexShader specEnvVertShader("../Shaders/SpecularEnvMapVertShader.cso");
    specEnvVertShader.CreateShader(AppRenderer);
    PixelShader specEnvPixShader("../Shaders/SpecularEnvMapPixShader.cso");
    specEnvPixShader.CreateShader(AppRenderer);

    VertexShader specBRDFVertShader("../Shaders/SpecularBRDFVertShader.cso");
    specBRDFVertShader.CreateShader(AppRenderer);
    PixelShader specBRDFPixShader("../Shaders/SpecularBRDFPixShader.cso");
    specBRDFPixShader.CreateShader(AppRenderer);




#pragma endregion

#pragma region Scene stuff
    //Scene
    //Camera test
    AppRenderer->SetViewport(1024, 1024);
    Transform CamTes;
    CamTes.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    CamTes.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    OrthoCamera cam(CamTes, Vector3(-1, -1, 0.0f), Vector3(1, 1, 1), Vector2(1024, 1024));
    PerspectiveCamera pCam(CamTes, 60, 0.1f, 100000.0f, Vector2(AppWindow->GetWidth(), AppWindow->GetHeight()));

    Transform cube;
    cube.SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    cube.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    cube.SetScale(Vector3(1.f, 1.f, 1.f));
    cube.UpdateMatrix();

    Transform Skysphere;
    Skysphere.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    Skysphere.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    Skysphere.SetScale(Vector3(1.f, 1.f, 1.f));
    Skysphere.UpdateMatrix();

    Matrix4x4 ViewProj = pCam.GetCameraProjectionMatrix() * pCam.GetCameraViewMatrix().GetMat3x3().GetMat4x4();
    ViewProj = ViewProj.Transpose();
    Matrix4x4 MSkyWorld = Skysphere.GetModelMatrix();
    MSkyWorld = MSkyWorld.Transpose();


    Matrix4x4 MVP = pCam.GetCameraProjectionMatrix() * (pCam.GetCameraViewMatrix() * cube.GetModelMatrix());
    MVP = MVP.Transpose();

    Vector3 lightDirNorm = Vector3::zero();
    lightDirNorm.y = -1.0f;

    Transform pLight;
    pLight.SetPosition(Vector3(-3.0f, 0.0f, 5.0f));
    Vector3 plColor = Vector3(0.8f, 0.0f, 0.6f);
    PointLight pointLight1(pLight, 5.0f, plColor, 4.0f);
#pragma endregion
    
#pragma region Cbuffer setup temp
    ///cbuffer stuff

    struct cbuffer
    {
        Vector4 time;
        float MVP[16];
        float MW[16];
        float MC[16];
        float MNorm[16];
        Vector4 light;
        Vector4 Ambient;
        Vector4 CamPosWS;
        Vector4 PointLightPos;
        Vector4 PointLightColor;
    };

    CBuffer<cbuffer> buffer;
    buffer.BufferData.time.x = 0;
    buffer.BufferData.light = lightDirNorm.GetVec4(false);
    buffer.BufferData.Ambient = Vector4(83.0f / 255, 83.0f / 255, 133.0f / 255.0f, 1);
    buffer.BufferData.CamPosWS = CamTes.GetPosition().GetVec4(true);
    buffer.BufferData.PointLightPos = pointLight1.GetPosition().GetVec4(true);
    buffer.BufferData.PointLightPos.w = pointLight1.GetLightRadius();
    buffer.BufferData.PointLightColor = pointLight1.GetColor().GetVec4(false);
    buffer.BufferData.PointLightColor.w = pointLight1.GetIntensity();
    Matrix4x4 MWorld = cube.GetModelMatrix();
    Matrix4x4 MNormal = MWorld.GetMat3x3().GetInverse().GetMat4x4();

    Matrix4x4 MView = CamTes.GetModelMatrix();

    MWorld.Transpose();
    MView.Transpose();

    MVP.GetMatrixFloatArray(buffer.BufferData.MVP);
    MWorld.GetMatrixFloatArray(buffer.BufferData.MW);
    MView.GetMatrixFloatArray(buffer.BufferData.MC);
    MNormal.GetMatrixFloatArray(buffer.BufferData.MNorm);

    buffer.CreateBuffer(AppRenderer);
    buffer.BindBuffer(AppRenderer, 0);

    struct skyboxBuffer
    {
        float MViewP[16];
        float MW[16];
    };

    CBuffer<skyboxBuffer> skybuffer;
    ViewProj.GetMatrixFloatArray(skybuffer.BufferData.MViewP);
    MSkyWorld.GetMatrixFloatArray(skybuffer.BufferData.MW);
    skybuffer.CreateBuffer(AppRenderer);
    skybuffer.BindBuffer(AppRenderer, 1);


#pragma endregion


#pragma region Generate cubemap test
    TextureCube HDRICubeMap;
    HDRICubeMap.CreateCubeMapRenderTexture(AppRenderer, 512, 512);
    HDRICubeMap.RenderHDRIToCubeMap(AppRenderer, AppWindow, HDRI);
    HDRICubeMap.BindTexture(AppRenderer, 3);
    DiffuseTex.BindTexture(AppRenderer, 0);

    TextureCube IrradianceCubeMap;
    IrradianceCubeMap.CreateCubeMapRenderTexture(AppRenderer, 128, 128);
    IrradianceCubeMap.RenderPrefilteredCubeMap(AppRenderer, AppWindow, HDRICubeMap, hdrVertShader, hdrPixShader);
    IrradianceCubeMap.BindTexture(AppRenderer, 3);
    //genCubeMap.BindTexture(AppRenderer, 3);

    /**/
    TextureCube SpecularEnvMap;
    SpecularEnvMap.CreateCubeMapRenderTexture(AppRenderer,128, 128, 8, 4, DXGI_FORMAT_R8G8B8A8_UNORM, true, 5);
    SpecularEnvMap.RenderPrefilteredCubeWithMips(AppRenderer, AppWindow, HDRICubeMap, specEnvVertShader, specEnvPixShader);
    SpecularEnvMap.BindTexture(AppRenderer, 4);

    Texture2D SpecularIntegrationBRDF;
    SpecularIntegrationBRDF.CreateRenderTexture(AppRenderer,512, 512, 16, 2, DXGI_FORMAT_R16G16_UNORM);
    //SpecularIntegrationBRDF.CreateRenderTexture(AppRenderer, 128, 128, 8, 4, DXGI_FORMAT_R8G8B8A8_UNORM);
    SpecularIntegrationBRDF.RenderToTexture(AppRenderer, AppWindow, specBRDFVertShader, specBRDFPixShader);
    SpecularIntegrationBRDF.BindTexture(AppRenderer, 5);

#pragma endregion


#pragma region Vertex and Pixel Shader switch
    ///*
    baseVertShader.BindShader(AppRenderer);
    basePixShader.BindShader(AppRenderer);
    sphereMesh.BindMesh(0, AppRenderer);
    //*/
#pragma endregion

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
        //rot.y += 0.1f * deltaTime;
        //rot.y = fmod(rot.y, 360.0f);

        //updating cbuffer struct

        cube.SetRotation(rot);
        cube.UpdateMatrix();
        CamTes.UpdateMatrix();
        
        MVP = pCam.GetCameraProjectionMatrix() * (pCam.GetCameraViewMatrix() * cube.GetModelMatrix());
        MVP = MVP.Transpose();
        MVP.GetMatrixFloatArray(buffer.BufferData.MVP);

        MWorld = cube.GetModelMatrix();
        MNormal = MWorld.GetMat3x3().GetInverse().GetMat4x4();
        MView = CamTes.GetModelMatrix().Transpose();
        
        MWorld = MWorld.Transpose();
        MWorld.GetMatrixFloatArray(buffer.BufferData.MW);
        MView.GetMatrixFloatArray(buffer.BufferData.MC);
        MNormal.GetMatrixFloatArray(buffer.BufferData.MNorm);

        buffer.BufferData.CamPosWS = CamTes.GetPosition().GetVec4(true);
        buffer.BufferData.time.x += deltaTime;
        buffer.BufferData.light = lightDirNorm.GetVec4(false);

        buffer.BufferData.PointLightPos = pointLight1.GetPosition().GetVec4(true);
        buffer.BufferData.PointLightPos.w = pointLight1.GetLightRadius();
        //buffer.BufferData.PointLightColor = pointLight1.GetColor().GetVec4(false);
        buffer.BufferData.PointLightColor.w = pointLight1.GetIntensity();
        buffer.UpdateBuffer(AppRenderer);

        ViewProj = pCam.GetCameraProjectionMatrix() * pCam.GetCameraViewMatrix().GetMat3x3().GetMat4x4();;
        ViewProj = ViewProj.Transpose();
        ViewProj.GetMatrixFloatArray(skybuffer.BufferData.MViewP);
        MSkyWorld = Skysphere.GetModelMatrix();
        MSkyWorld = MSkyWorld.Transpose();
        MSkyWorld.GetMatrixFloatArray(skybuffer.BufferData.MW);
        skybuffer.UpdateBuffer(AppRenderer);

        AppRenderer->ClearBackbuffer();
        AppRenderer->rasterizerDesc.CullMode = D3D11_CULL_FRONT;
        AppRenderer->UpdateRasterizerState();

        baseVertShader.BindShader(AppRenderer);
        basePixShader.BindShader(AppRenderer);
        sphereMesh.BindMesh(0, AppRenderer);
        IrradianceCubeMap.BindTexture(AppRenderer, 3);
        AppRenderer->gfxContext->DrawIndexed(sphereMesh.GetIndexListSize(0), 0, 0);

        //Skybox Draw
        SkyboxVertShader.BindShader(AppRenderer);
        SkyboxPixShader.BindShader(AppRenderer);
        Skybox.BindMesh(0, AppRenderer);
        HDRICubeMap.BindTexture(AppRenderer, 3);
        skybuffer.BindBuffer(AppRenderer, 1);
        AppRenderer->rasterizerDesc.CullMode = D3D11_CULL_BACK;
        AppRenderer->UpdateRasterizerState();
        AppRenderer->gfxContext->DrawIndexed(Skybox.GetIndexListSize(0), 0, 0);

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

            float* ac[3] = { &buffer.BufferData.PointLightColor.x, &buffer.BufferData.PointLightColor.x , &buffer.BufferData.PointLightColor.z};

            ImGui::Begin("Transforms");                         


            ImGui::Text("Cube Transform:");
            ImGui::SliderFloat3("Position", *p, -100, 100);            
            ImGui::SliderFloat3("Rotation", *r, -180, 180);           
            ImGui::SliderFloat3("Scale", *s, -100, 100);            
            cube.SetPosition(pos);
            cube.SetRotation(rot);
            cube.SetScale(scale);

            
            ImGui::Text("Camera Transform:");
            ImGui::SliderFloat3("Cam Position", *pC, -100, 100);
            ImGui::SliderFloat3("Cam Rotation", *rC, -180, 180);
            CamTes.SetPosition(posC);
            CamTes.SetRotation(rotC);

            ImGui::Text("Light:");
            ImGui::SliderFloat3("Direction", *lD, -1, 1);
            ImGui::ColorEdit3("Point Light color", *ac); // Edit 3 floats representing a color
            
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
