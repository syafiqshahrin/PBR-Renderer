#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include "Renderer.h"
#include "Window.h"
#include "Debug.h"
#include <comdef.h>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "Matrix.h"
#include "Vector.h"
#include "CBuffer.h"

Texture2D::Texture2D()
{
    FilePath = "";
    IsRenderTexture = true;
    HDR = false;
}

Texture2D::Texture2D(std::string filepath, int c, bool hdr)
{
	FilePath = filepath;
    IsRenderTexture = false;
    component = c;
    HDR = hdr;
}

Texture2D::~Texture2D()
{
    ReleaseTexture();
}

bool Texture2D::CreateTextureFromFile(Renderer* renderer, int bitsperpixel,  bool genMips, DXGI_FORMAT format)
{
    IsRenderTexture = false;
    LoadTextureFromFile();
   

    if (!genMips)
    {
        TextureDesc.Width = TexDimensionsW;
        TextureDesc.Height = TexDimensionsH;
        TextureDesc.MipLevels = 1;
        TextureDesc.ArraySize = 1;
        TextureDesc.Format = format;
        TextureDesc.SampleDesc.Count = 1;
        TextureDesc.SampleDesc.Quality = 0;
        TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
        TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        TextureDesc.MiscFlags = 0;
        TextureDesc.CPUAccessFlags = 0;

        //only need subresource if not creating texture w generated mips
        D3D11_SUBRESOURCE_DATA texSubResourceData;
        if (HDR)
        {
            texSubResourceData.pSysMem = TextureData ;

        }
        else
        {
            texSubResourceData.pSysMem = TextureData;
        }
        texSubResourceData.SysMemPitch = TexDimensionsW * component * (bitsperpixel / 8);
        texSubResourceData.SysMemSlicePitch = 0;

        HRESULT hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, &texSubResourceData, &Texture2DResource);
        if (FAILED(hr))
        {
            _com_error error(hr);
            LPCTSTR errorText = error.ErrorMessage();
            DEBUG("Failed creating texture2d resource - " << errorText);

            //return -1;
            return false;
        }


        hr = renderer->gfxDevice->CreateShaderResourceView(Texture2DResource.Get(), nullptr, TextureShaderView.GetAddressOf());
        if (FAILED(hr))
        {
            _com_error error(hr);
            LPCTSTR errorText = error.ErrorMessage();
            DEBUG("Failed creating texture2d shader resource view - " << errorText);

            //return -1;
        }

    }
    else
    {
        TextureDesc.Width = TexDimensionsW;
        TextureDesc.Height = TexDimensionsH;
        TextureDesc.MipLevels = 0;
        TextureDesc.ArraySize = 1;
        TextureDesc.Format = format;
        TextureDesc.SampleDesc.Count = 1;
        TextureDesc.SampleDesc.Quality = 0;
        TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
        TextureDesc.CPUAccessFlags = 0;

        HRESULT hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, nullptr, &Texture2DResource);
        if (FAILED(hr))
        {
            _com_error error(hr);
            LPCTSTR errorText = error.ErrorMessage();
            DEBUG("Failed creating texture2d resource - " << errorText);

            //return -1;
            return false;
        }

        renderer->gfxContext->UpdateSubresource(Texture2DResource.Get(), 0, nullptr, TextureData, RowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC textviewdesc;
        textviewdesc.Format = TextureDesc.Format;
        textviewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textviewdesc.Texture2D.MipLevels = -1;
        textviewdesc.Texture2D.MostDetailedMip = 0;

        hr = renderer->gfxDevice->CreateShaderResourceView(Texture2DResource.Get(), &textviewdesc, TextureShaderView.GetAddressOf());
        if (FAILED(hr))
        {
            _com_error error(hr);
            LPCTSTR errorText = error.ErrorMessage();
            DEBUG("Failed creating texture2d shader resource view - " << errorText);

            //return -1;
        }
        renderer->gfxContext->GenerateMips(TextureShaderView.Get());
    }


    //generate mips
    if (HDR)
    {
        free(TextureDataF);
    }
    else
    {
        free(TextureData);
    }
    

	return true;
}

bool Texture2D::CreateRenderTexture(Renderer* renderer, int w, int h, int bitsperpixel, int c, DXGI_FORMAT format)
{
    IsRenderTexture = true;

    TextureDesc.Width = w;
    TextureDesc.Height = h;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = format;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    TextureDesc.MiscFlags = 0;
    TextureDesc.CPUAccessFlags = 0;
    return false;

    HRESULT hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, nullptr, &Texture2DResource);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating texture2d resource - " << errorText);

        //return -1;
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
    RenderTargetDesc.Format = format;
    RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RenderTargetDesc.Texture2D.MipSlice = 0;

    hr = renderer->gfxDevice->CreateRenderTargetView(Texture2DResource.Get(), &RenderTargetDesc, &TextureRenderView);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating texture2d render target view - " << errorText);

        //return -1;
        return false;
    }

    hr = renderer->gfxDevice->CreateShaderResourceView(Texture2DResource.Get(), nullptr, TextureShaderView.GetAddressOf());
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating texture2d shader resource view - " << errorText);

        //return -1;
    }

}

void Texture2D::BindTexture(Renderer* renderer, int bindslot) const
{
    renderer->gfxContext->PSSetShaderResources(bindslot, 1, TextureShaderView.GetAddressOf());
}

void Texture2D::BindAsRenderTarget(Renderer* renderer) const
{
    if(IsRenderTexture)
        renderer->gfxContext->OMSetRenderTargets(1, TextureRenderView.GetAddressOf(), nullptr);
}

void Texture2D::ReleaseTexture()
{
	//if(TextureData != nullptr)
		//free(TextureData);
	Texture2DResource->Release();
    TextureShaderView->Release();
    if(IsRenderTexture)
        TextureRenderView->Release();
}

void Texture2D::LoadTextureFromFile()
{
    if (HDR)
    {
        stbi_set_flip_vertically_on_load(true);
        //TextureDataF = stbi_loadf(FilePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, component);
        TextureData = stbi_load(FilePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, component);
    }
    else
    {
        TextureData = stbi_load(FilePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, component);
    }
    RowPitch = TexDimensionsW * component;
}




TextureCube::TextureCube()
{
    PartialFilePath = "";
    IsRenderTexture = false;
    HDR = false;
}

TextureCube::TextureCube(std::string parfilepath, int c, bool hdr)
{
    PartialFilePath = parfilepath;
    IsRenderTexture = false;
    component = c;
    HDR = hdr;
}

TextureCube::~TextureCube()
{
    ReleaseTexture();
}

bool TextureCube::CreateTextureFromFile(Renderer* renderer, int bitsperpixel, DXGI_FORMAT format)
{
    IsRenderTexture = false;
    HRESULT hr;
    LoadTextureFromFile();
   
    TextureDesc.Width = TexDimensionsW;
    TextureDesc.Height = TexDimensionsH;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 6;
    TextureDesc.Format = format;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    TextureDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA cubemapSubResourceData[6];

    for (int i = 0; i < 6; i++)
    {
        cubemapSubResourceData[i].pSysMem = TextureData[i];
        cubemapSubResourceData[i].SysMemPitch = RowPitch * (bitsperpixel / 8);
        cubemapSubResourceData[i].SysMemSlicePitch = 0;
    }

    hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, &cubemapSubResourceData[0], &Texture2DResource);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating cubemap texture - " << errorText);

        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC cubemapShaderViewDesc;
    cubemapShaderViewDesc.Format = TextureDesc.Format;
    cubemapShaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    cubemapShaderViewDesc.TextureCube.MipLevels = TextureDesc.MipLevels;
    cubemapShaderViewDesc.TextureCube.MostDetailedMip = 0;

    hr = renderer->gfxDevice->CreateShaderResourceView(Texture2DResource.Get(), &cubemapShaderViewDesc, &TextureShaderView);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating cubemap texture shader view - " << errorText);

        return false;
    }

    //freeing texture data from cpu side
    for (int i = 0; i < 6; i++)
    {
        if (HDR)
        {
            free(TextureDataF[i]);
        }
        else
        {
            free(TextureData[i]);
        }
    }
    //free(TextureData);

    return true;
}

bool TextureCube::CreateCubeMapRenderTexture(Renderer* renderer, int w, int h, int bitsperpixel, int c, DXGI_FORMAT format)
{

    //Fill out tex desc
    //Create texture resource
    //Create Shader view resource
    //Create 6 render view target - one for each face
    IsRenderTexture = true;
    HRESULT hr;

    TextureDesc.Width = w;
    TextureDesc.Height = h;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 6;
    TextureDesc.Format = format;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    TextureDesc.CPUAccessFlags = 0;
    
    hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, nullptr, &Texture2DResource);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating cubemap texture - " << errorText);

        return false;
    }
    
    D3D11_SHADER_RESOURCE_VIEW_DESC cubemapShaderViewDesc;
    cubemapShaderViewDesc.Format = format;
    cubemapShaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    cubemapShaderViewDesc.TextureCube.MipLevels = TextureDesc.MipLevels;
    cubemapShaderViewDesc.TextureCube.MostDetailedMip = 0;

    hr = renderer->gfxDevice->CreateShaderResourceView(Texture2DResource.Get(), &cubemapShaderViewDesc, &TextureShaderView);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating cubemap texture shader view - " << errorText);

        return false;
    }


    D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
    viewDesc.Format = format;
    viewDesc.Texture2DArray.ArraySize = 1;
    viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    
    for (int i = 0; i < 6; i++)
    {
        viewDesc.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
        hr = renderer->gfxDevice->CreateRenderTargetView(Texture2DResource.Get(), &viewDesc, TextureRenderView[i].GetAddressOf());
        if (FAILED(hr))
        {
            _com_error error(hr);
            LPCTSTR errorText = error.ErrorMessage();
            DEBUG("Failed creating cubemap texture render view - " << errorText);

            return false;
        }
    }
    
    return false;
}

void TextureCube::BindTexture(Renderer* renderer, int bindslot) const
{
    renderer->gfxContext->PSSetShaderResources(bindslot, 1, TextureShaderView.GetAddressOf());
}

void TextureCube::BindAsRenderTarget(Renderer* renderer, int face) const
{
    if (IsRenderTexture)
        renderer->gfxContext->OMSetRenderTargets(1, TextureRenderView[face].GetAddressOf(), nullptr);
}

void TextureCube::RenderHDRIToCubeMap(Renderer* renderer, Window* wndw, Texture2D const &HDRI)
{
    //Mesh cubeMesh("D:/Asset Files/Blender/FBX Files/UnitCube.gltf");
    Mesh cubeMesh("E:/My Documents/Assets/Blender/FBX/UnitCube.gltf");
    cubeMesh.CreateMeshFromFile(renderer);
    cubeMesh.BindMesh(0, renderer);

    VertexShader hdrVertShader("../Shaders/HDRConverterVertShader.cso");
    hdrVertShader.CreateShader(renderer);
    PixelShader hdrPixShader("../Shaders/HDRConverterPixShader.cso");
    hdrPixShader.CreateShader(renderer);

    hdrVertShader.BindShader(renderer);
    hdrPixShader.BindShader(renderer);

    Transform CamTransform;

    PerspectiveCamera persCam(CamTransform, 90, 0.1f, 100000.0f, Vector2(TextureDesc.Width, TextureDesc.Height));
    Matrix4x4 ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();

    struct buffer
    {
        float VP[16];
    };

    CBuffer<buffer> buf;
    ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();
    ViewP = ViewP.Transpose();
    ViewP.GetMatrixFloatArray(buf.BufferData.VP);
    buf.CreateBuffer(renderer);
    buf.BindBuffer(renderer, 1);

    renderer->SetViewport(TextureDesc.Width, TextureDesc.Height);
    renderer->rasterizerDesc.CullMode = D3D11_CULL_BACK;
    renderer->UpdateRasterizerState();
    HDRI.BindTexture(renderer, 0);
    Vector3 rots[6] =
    {
        Vector3(0.0f, 90.0f, 0.0f),
        Vector3(0.0f, -90.0f, 0.0f),
        Vector3(-90.0f, 0.0f, 0.0f),
        Vector3(90.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 180.0f, 0.0f)
    };

    for (int i = 0; i < 6; i++)
    {
        this->BindAsRenderTarget(renderer, i);
        CamTransform.SetRotation(rots[i]);
        CamTransform.UpdateMatrix();

        ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();
        ViewP = ViewP.Transpose();
        ViewP.GetMatrixFloatArray(buf.BufferData.VP);
        buf.UpdateBuffer(renderer);

        renderer->gfxContext->DrawIndexed(cubeMesh.GetIndexListSize(0), 0, 0);
    }

    renderer->rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    renderer->UpdateRasterizerState();
    renderer->SetViewport(wndw->GetWidth(), wndw->GetHeight());
    renderer->BindBackBufferAsRenderTarget();
    //this->BindTexture(renderer, 3);
}

void TextureCube::RenderPrefilteredCubeMap(Renderer* renderer, Window* wndw, TextureCube const& cubemap)
{
    //Mesh cubeMesh("D:/Asset Files/Blender/FBX Files/UnitCube.gltf");
    Mesh cubeMesh("E:/My Documents/Assets/Blender/FBX/UnitCube.gltf");
    cubeMesh.CreateMeshFromFile(renderer);
    cubeMesh.BindMesh(0, renderer);

    VertexShader hdrVertShader("../Shaders/PrefilterCubeMapVertShader.cso");
    hdrVertShader.CreateShader(renderer);
    PixelShader hdrPixShader("../Shaders/PrefilterCubeMapPixShader.cso");
    hdrPixShader.CreateShader(renderer);

    hdrVertShader.BindShader(renderer);
    hdrPixShader.BindShader(renderer);

    Transform CamTransform;

    PerspectiveCamera persCam(CamTransform, 90, 0.1f, 100000.0f, Vector2(TextureDesc.Width, TextureDesc.Height));
    Matrix4x4 ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();

    struct buffer
    {
        float VP[16];
    };

    CBuffer<buffer> buf;
    ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();
    ViewP = ViewP.Transpose();
    ViewP.GetMatrixFloatArray(buf.BufferData.VP);
    buf.CreateBuffer(renderer);
    buf.BindBuffer(renderer, 1);

    renderer->SetViewport(TextureDesc.Width, TextureDesc.Width);
    renderer->rasterizerDesc.CullMode = D3D11_CULL_BACK;
    renderer->UpdateRasterizerState();
    cubemap.BindTexture(renderer, 3);
    Vector3 rots[6] =
    {
        Vector3(0.0f, 90.0f, 0.0f),
        Vector3(0.0f, -90.0f, 0.0f),
        Vector3(-90.0f, 0.0f, 0.0f),
        Vector3(90.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 180.0f, 0.0f)
    };

    for (int i = 0; i < 6; i++)
    {
        this->BindAsRenderTarget(renderer, i);
        CamTransform.SetRotation(rots[i]);
        CamTransform.UpdateMatrix();

        ViewP = persCam.GetCameraProjectionMatrix() * persCam.GetCameraViewMatrix();
        ViewP = ViewP.Transpose();
        ViewP.GetMatrixFloatArray(buf.BufferData.VP);
        buf.UpdateBuffer(renderer);

        renderer->gfxContext->DrawIndexed(cubeMesh.GetIndexListSize(0), 0, 0);
    }

    renderer->rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    renderer->UpdateRasterizerState();
    renderer->SetViewport(wndw->GetWidth(), wndw->GetHeight());
    renderer->BindBackBufferAsRenderTarget();
    buf.UnbindBuffer(renderer, 1);
}

void TextureCube::ReleaseTexture()
{
    Texture2DResource->Release();
    TextureShaderView->Release();
}

void TextureCube::LoadTextureFromFile()
{
    for (int i = 0; i < 6; i++)
    {
        std::string completePath = PartialFilePath + std::to_string(i) + ".png"; //probably change to png
        if (HDR)
        {
            TextureDataF[i] = stbi_loadf(completePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, component);
        }
        else
        {
            TextureData[i] = stbi_load(completePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, component);
        }
    }
    RowPitch = TexDimensionsW * component;
}
