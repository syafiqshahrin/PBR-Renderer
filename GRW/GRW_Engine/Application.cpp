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
#include "Sampler.h"
#include "AssetManager.h"

Application::Application(LPCWSTR AppTitle, int w, int h, HINSTANCE hInstance)
{
	AppWindow = new Window(AppTitle, w, h, hInstance);
	AppRenderer = new Renderer(AppWindow->GetWindHandle(), w, h, true);
    AssetManager::SetRenderer(AppRenderer);
}

Application::~Application()
{
	delete AppWindow;
	delete AppRenderer;
    delete AssetManager::GetAssetManager();
}

void Application::StartApplication()
{
    AssetManager::GetAssetManager()->LoadAllAssets();
    Deltatime = 0;
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

    Texture2D *DiffuseTex = AssetManager::GetAssetManager()->GetAsset<Texture2D>("TexturedSurface2_basecolor.png");
    DiffuseTex->BindTexture(AppRenderer, 0);

    Texture2D *NormalTex = AssetManager::GetAssetManager()->GetAsset<Texture2D>("TexturedSurface2_normal.png");
    NormalTex->BindTexture(AppRenderer, 1);

    Texture2D *RMATex = AssetManager::GetAssetManager()->GetAsset<Texture2D>("TexturedSurface2_RMA.png");
    RMATex->BindTexture(AppRenderer, 2);

    Texture2D *HDRI = AssetManager::GetAssetManager()->GetAsset<Texture2D>("resting_place_2_2k.hdr");

#pragma endregion

#pragma region Sampler Setup temp
    Sampler DefaultSampler;
    DefaultSampler.CreateSampler(AppRenderer);
    DefaultSampler.BindSampler(0, AppRenderer);
#pragma endregion

#pragma region Mesh Loading and mesh binding
    Mesh *sphereMesh = AssetManager::GetAssetManager()->GetAsset<Mesh>("SphereTest.gltf");
    Mesh *Skybox = AssetManager::GetAssetManager()->GetAsset<Mesh>("SphereTest.gltf");

#pragma endregion

#pragma region Vertex and Pixel Shader setup temp
    VertexShader* baseVertShader = AssetManager::GetAssetManager()->GetAsset<VertexShader>("BaseVertexShader.cso");
    PixelShader* basePixShader = AssetManager::GetAssetManager()->GetAsset<PixelShader>("BasePixelShader.cso");

    VertexShader* SkyboxVertShader = AssetManager::GetAssetManager()->GetAsset<VertexShader>("SkyboxVertShader.cso");
    PixelShader* SkyboxPixShader = AssetManager::GetAssetManager()->GetAsset<PixelShader>("SkyboxPixShader.cso");

    VertexShader* hdrVertShader = AssetManager::GetAssetManager()->GetAsset<VertexShader>("PrefilterCubeMapVertShader.cso");
    PixelShader* hdrPixShader = AssetManager::GetAssetManager()->GetAsset<PixelShader>("PrefilterCubeMapPixShader.cso");

    VertexShader* specEnvVertShader = AssetManager::GetAssetManager()->GetAsset<VertexShader>("SpecularEnvMapVertShader.cso");
    PixelShader* specEnvPixShader = AssetManager::GetAssetManager()->GetAsset<PixelShader>("SpecularEnvMapPixShader.cso");

    VertexShader* specBRDFVertShader = AssetManager::GetAssetManager()->GetAsset<VertexShader>("SpecularBRDFVertShader.cso");
    PixelShader* specBRDFPixShader = AssetManager::GetAssetManager()->GetAsset<PixelShader>("SpecularBRDFPixShader.cso");

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


#pragma region Generate cubemap
    TextureCube HDRICubeMap;
    HDRICubeMap.CreateCubeMapRenderTexture(AppRenderer, 512, 512);
    HDRICubeMap.RenderHDRIToCubeMap(AppRenderer, AppWindow, *HDRI);
    HDRICubeMap.BindTexture(AppRenderer, 3);
    DiffuseTex->BindTexture(AppRenderer, 0);

    TextureCube IrradianceCubeMap;
    IrradianceCubeMap.CreateCubeMapRenderTexture(AppRenderer, 128, 128);
    IrradianceCubeMap.RenderPrefilteredCubeMap(AppRenderer, AppWindow, HDRICubeMap, *hdrVertShader, *hdrPixShader);
    IrradianceCubeMap.BindTexture(AppRenderer, 3);
    //genCubeMap.BindTexture(AppRenderer, 3);

    /**/
    TextureCube SpecularEnvMap;
    SpecularEnvMap.CreateCubeMapRenderTexture(AppRenderer,128, 128, 8, 4, DXGI_FORMAT_R8G8B8A8_UNORM, true, 5);
    SpecularEnvMap.RenderPrefilteredCubeWithMips(AppRenderer, AppWindow, HDRICubeMap, *specEnvVertShader, *specEnvPixShader);
    SpecularEnvMap.BindTexture(AppRenderer, 4);

    Texture2D SpecularIntegrationBRDF;
    SpecularIntegrationBRDF.CreateRenderTexture(AppRenderer,512, 512, 16, 2, DXGI_FORMAT_R16G16_UNORM);
    SpecularIntegrationBRDF.RenderToTexture(AppRenderer, AppWindow, *specBRDFVertShader, *specBRDFPixShader);
    SpecularIntegrationBRDF.BindTexture(AppRenderer, 5);

#pragma endregion


#pragma region Vertex and Pixel Shader switch
    ///*
    baseVertShader->BindShader(AppRenderer);
    basePixShader->BindShader(AppRenderer);
    sphereMesh->BindMesh(0, AppRenderer);
    //*/
#pragma endregion

    std::vector<std::string> Textures;
    std::vector<std::string> Meshes;
    AssetManager::GetAssetManager()->GetAllLoadedTextureNames(Textures);
    AssetManager::GetAssetManager()->GetAllLoadedMeshNames(Meshes);
    int SelectedTextureIndex = 0;
    int SelectedMeshIndex = 0;
    
    //App loop
    while (true)
    {
        std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        if (AppWindow->ProcessMessages() == -1)
        {
            break;
        }


#pragma region Logic Update
        Vector3 rot = cube.GetRotation();
        //rot.z += 0.1f * deltaTime;
        //rot.x += 0.2f * deltaTime;
        rot.y += 0.1f * Deltatime;
        rot.y = fmod(rot.y, 360.0f);

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
        buffer.BufferData.time.x += Deltatime;
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
#pragma endregion
       
#pragma region Draw Loop
        AppRenderer->ClearBackbuffer();
        AppRenderer->rasterizerDesc.CullMode = D3D11_CULL_FRONT;
        AppRenderer->UpdateRasterizerState();
        //Testing switching textures
        DiffuseTex = AssetManager::GetAssetManager()->GetAsset<Texture2D>(Textures[SelectedTextureIndex]);
        DiffuseTex->BindTexture(AppRenderer, 0);
        //

        baseVertShader->BindShader(AppRenderer);
        basePixShader->BindShader(AppRenderer);

        //Testing switching meshes
        sphereMesh = AssetManager::GetAssetManager()->GetAsset<Mesh>(Meshes[SelectedMeshIndex]);
        sphereMesh->BindMesh(0, AppRenderer);
        //
        IrradianceCubeMap.BindTexture(AppRenderer, 3);
        AppRenderer->gfxContext->DrawIndexed(sphereMesh->GetIndexListSize(0), 0, 0);

        //Skybox Draw
        SkyboxVertShader->BindShader(AppRenderer);
        SkyboxPixShader->BindShader(AppRenderer);
        Skybox->BindMesh(0, AppRenderer);
        HDRICubeMap.BindTexture(AppRenderer, 3);
        skybuffer.BindBuffer(AppRenderer, 1);
        AppRenderer->rasterizerDesc.CullMode = D3D11_CULL_BACK;
        AppRenderer->UpdateRasterizerState();
        AppRenderer->gfxContext->DrawIndexed(Skybox->GetIndexListSize(0), 0, 0);
#pragma endregion
        //ImGui Stuff

#pragma region IMGUI Stuff
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

            float* ac[3] = { &buffer.BufferData.PointLightColor.x, &buffer.BufferData.PointLightColor.x , &buffer.BufferData.PointLightColor.z };

            if(ImGui::Begin("Transforms"))
            {

                ImGui::Text("Object Transform:");
                ImGui::DragFloat3("Position", *p, 0.1f);
                ImGui::DragFloat3("Rotation", *r, 1.0f);
                ImGui::DragFloat3("Scale", *s, 0.1f);
                cube.SetPosition(pos);
                cube.SetRotation(rot);
                cube.SetScale(scale);


                ImGui::Text("Camera Transform:");
                ImGui::DragFloat3("Cam Position", *pC, 0.1f);
                ImGui::DragFloat3("Cam Rotation", *rC, 1.0f);
                CamTes.SetPosition(posC);
                CamTes.SetRotation(rotC);

                ImGui::Text("Light:");
                ImGui::SliderFloat3("Direction", *lD, -1, 1);
                ImGui::ColorEdit3("Point Light color", *ac); // Edit 3 floats representing a color

                //ImGui::SameLine();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
            else
            {
                ImGui::End();
            }

            if (ImGui::Begin("Asset List"))
            {
                if (ImGui::CollapsingHeader("Loaded Textures"))
                {
                    if (ImGui::BeginListBox("##Textures", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing() ) ) )
                    {
                        
                        for (int i = 0; i < Textures.size(); i++)
                        {
                            const bool is_selected = (SelectedTextureIndex == i);
                            if (ImGui::Selectable(Textures[i].c_str(), is_selected))
                                SelectedTextureIndex = i;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }

                }

                if (ImGui::CollapsingHeader("Loaded Meshes"))
                {
                    if (ImGui::BeginListBox("##Meshes", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
                    {

                        for (int i = 0; i < Meshes.size(); i++)
                        {
                            const bool is_selected = (SelectedMeshIndex == i);
                            if (ImGui::Selectable(Meshes[i].c_str(), is_selected))
                                SelectedMeshIndex = i;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }

                }

                ImGui::End();
            }
            else
            {
                ImGui::End();
            }

           // ImGui::Begin("Transforms", );

        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

        AppRenderer->UpdateSwapchain();


        std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        Deltatime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count() / 1000.0f;
        //DEBUG("Time difference = " << deltaTime);
    }
    return 0;

}
