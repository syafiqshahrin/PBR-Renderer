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
    //load shader file onto gpu? Only needs to do this once on scene load
  //Bind shader to context - do this everytime we need to change shaders

  //Shader class - stores path, has shaderID for both vertex and pixel shader, dictionaries for shader data (one dictionary for each type)
  //LoadedShaderCollection Class - has a dictionary for both vertex and shader dx11 resource, key is path and resource is value

  //Vert Shader

    std::ifstream f("D:/Asset Files/Blender/FBX Files/Testgltf.gltf");
    nlohmann::json dataJson = nlohmann::json::parse(f);

    std::string s = dataJson["buffers"][0]["uri"];
    std::byte bytes[4];
    bytes[0] = (std::byte)s.c_str()[37];
    bytes[1] = (std::byte)s.c_str()[38];
    bytes[2] = (std::byte)s.c_str()[39];
    bytes[3] = (std::byte)s.c_str()[40];
    //std::memcpy(bytes, s.c_str(), 4);
    float testFloat;
    std::memcpy(&testFloat, bytes, 4);
    //DEBUG(s.c_str()[36] << "," << s.c_str()[37]);
    DEBUG(testFloat);

    HRESULT hr;
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
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    AppRenderer->gfxDevice->CreateInputLayout(inLayoutDesc, 2, vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), &inputLayoutHandle);
    AppRenderer->gfxContext->IASetInputLayout(inputLayoutHandle.Get());

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

    hr = AppRenderer->gfxDevice->CreateBuffer(&verBufferDesc, &srd, &vertBuffer);
    if (FAILED(hr))
    {
        std::cout << "Failed creatiing buffer" << std::endl;

        return -1;
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
    indBufferDesc.ByteWidth = sizeof(meshIndicesData); //size of the whole array
    indBufferDesc.StructureByteStride = sizeof(UINT); //stride of each vertex 
    indBufferDesc.CPUAccessFlags = 0u;
    indBufferDesc.MiscFlags = 0u;

    srd.pSysMem = meshIndicesData;

    AppRenderer->gfxDevice->CreateBuffer(&indBufferDesc, &srd, &indexBuffer);
    AppRenderer->gfxContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //set primitive topology type
    AppRenderer->gfxContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
    CamTes.SetPosition(Vector3(5.0f,5.0f, 0.0f));
    OrthoCamera cam(CamTes, Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWindow->GetWidth(), AppWindow->GetHeight()));
    
    float test[16];

    Transform cube;
    cube.SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    cube.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    cube.SetScale(Vector3(2.f, 2.f, 2.f));
    cube.UpdateMatrix();

    Matrix4x4 MVP = cam.GetCameraProjectionMatrix() * (cam.GetCameraViewMatrix() * cube.GetModelMatrix());

    MVP = MVP.Transpose();

    ///cbuffer stuff
    Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;

    struct Cbuffer
    {
        Vector4 time;
        float MVP[16];
    };

    Cbuffer cbuffer;
    cbuffer.time.x = 0;
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

    hr = AppRenderer->gfxDevice->CreateBuffer(&cbDesc, &data, &constBuffer);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create cbuffer:" << errorText);
    }

    AppRenderer->gfxContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
    AppRenderer->gfxContext->PSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());



    //App loop
    float deltaTime = 0;
    while (AppWindow->ProcessMessages() != -1)
    {
        std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        
        //render loop stuff
        //draw triangle
        Vector3 rot = cube.GetRotation();
        rot.z += 0.1f * deltaTime;
        rot.x += 0.1f * deltaTime;
        rot.y += 0.1f * deltaTime;
        //DEBUG("Angle = " << rot.z);
        cube.SetRotation(rot);
        cube.UpdateMatrix();
        Matrix4x4 MVP = Matrix4x4::GetOrthoProjectionMatrix(Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWindow->GetWidth(), AppWindow->GetHeight())) * (CamTes.GetModelMatrix().GetInverse() * cube.GetModelMatrix());
        MVP = MVP.Transpose();
        MVP.GetMatrixFloatArray(cbuffer.MVP);
        cbuffer.time.x += deltaTime;

        /*
        for (int i = 0; i < 4; i++)
        {
            DEBUG(cbuffer.MVP[(i * 4)] << "," << cbuffer.MVP[(i * 4) + 1] << "," << cbuffer.MVP[(i * 4) + 2] << "," << cbuffer.MVP[(i * 4) + 3]);
        }
        */

        //update constant buffer data
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        AppRenderer->gfxContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &cbuffer, sizeof(cbuffer));
        AppRenderer->gfxContext->Unmap(constBuffer.Get(), 0);

        AppRenderer->ClearBackbuffer();
        AppRenderer->gfxContext->DrawIndexed(sizeof(meshIndicesData) / sizeof(UINT), 0, 0);
        AppRenderer->UpdateSwapchain();

        std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count() / 1000.0f;
        //DEBUG("Time difference = " << deltaTime);
    }
    return 0;
}
