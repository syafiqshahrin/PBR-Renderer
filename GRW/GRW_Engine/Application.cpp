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
#include <bitset>
#include "GLTFMeshLoader.h"

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
    /*
    std::string strMap = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

    std::map<char, int> base64Map;

    for (int i = 0; i < 64; i++)
    {
        base64Map.insert({ strMap[i], i });
    }

    //std::ifstream f("E:/My Documents/Assets/Blender/FBX/TestGLTF.gltf");
    std::ifstream f("D:/Asset Files/Blender/FBX Files/Testgltf.gltf");
    nlohmann::json dataJson = nlohmann::json::parse(f);

    std::string uri = dataJson["buffers"][0]["uri"];

    //for every 4 chars in uri
        //create an int store and set it to 0
        //for j = 0, j < 4, j++
            //int temp = map index for i+j
            //store = store OR temp
            //store = store bitshift left by 6 
        //retrieve byte from store and add it to byte array
            //bit shift store to the right by 8
    
    //stores 3 bytes
    int store = 0;

    int t = base64Map[uri[37]];
    //or index and then left shift by 6
    store = store | t;
    store = store << 6;

    t = base64Map[uri[38]];

    store = store | t;
    store = store << 6;

    t = base64Map[uri[39]];

    store = store | t;
    store = store << 6;

    t = base64Map[uri[40]];

    store = store | t;

    //extract byte from int store
    char c3 = store & 0xFF;
    store = store >> 8;
    char c2 = store & 0xFF;
    store = store >> 8;
    char c1 = store & 0xFF;


    store = 0;

    t = base64Map[uri[41]];

    store = store | t;
    store = store << 6;

    t = base64Map[uri[42]];

    store = store | t;
    store = store << 6;

    t = base64Map[uri[43]];

    store = store | t;
    store = store << 6;

    t = base64Map[uri[44]];

    store = store | t;

    char c6 = store & 0xFF;
    store = store >> 8;
    char c5 = store & 0xFF;
    store = store >> 8;
    char c4 = store & 0xFF;

    std::bitset<8> x(c1);
    std::bitset<8> y(c2);
    std::bitset<8> z(c3);
    std::bitset<8> a(c4);

    std::byte bytesarray[4] = { (std::byte)c1, (std::byte)c2, (std::byte)c3, (std::byte)c4 };
    float test;
    std::memcpy(&test, bytesarray, sizeof(float));

    DEBUG(x);
    DEBUG(y);
    DEBUG(z);
    DEBUG(a);
    DEBUG("float:" << test);
    */
    
    GLTFMeshLoader meshLoader("D:/Asset Files/Blender/FBX Files/Testgltf.gltf");
    //GLTFMeshLoader meshLoader("E:/My Documents/Assets/Blender/FBX/TestGLTF.gltf");

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
    CamTes.SetPosition(Vector3(0.0f,0.0f, 0.0f));
    OrthoCamera cam(CamTes, Vector3(-10, -10, 0.0f), Vector3(10, 10, 10), Vector2(AppWindow->GetWidth(), AppWindow->GetHeight()));
    

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
        AppRenderer->gfxContext->DrawIndexed(indArray.size() , 0, 0);
        AppRenderer->UpdateSwapchain();

        std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count() / 1000.0f;
        //DEBUG("Time difference = " << deltaTime);
    }
    return 0;
}
