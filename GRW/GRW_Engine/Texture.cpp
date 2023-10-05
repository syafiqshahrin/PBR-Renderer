#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include "Renderer.h"
#include "Debug.h"
#include <comdef.h>

Texture2D::Texture2D()
{
    FilePath = "";
    IsRenderTexture = true;
}

Texture2D::Texture2D(std::string filepath)
{
	FilePath = filepath;
    IsRenderTexture = false;
}

Texture2D::~Texture2D()
{
    ReleaseTexture();
}

bool Texture2D::CreateTextureFromFile(Renderer* renderer, bool genMips, DXGI_FORMAT format)
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
        texSubResourceData.pSysMem = TextureData;
        texSubResourceData.SysMemPitch = RowPitch;
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
   
    free(TextureData);

	return true;
}

bool Texture2D::CreateRenderTexture(Renderer* renderer, int w, int h, DXGI_FORMAT format)
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

}

void Texture2D::BindTexture(Renderer* renderer, int bindslot)
{
    renderer->gfxContext->PSSetShaderResources(bindslot, 1, TextureShaderView.GetAddressOf());
}

void Texture2D::BindAsRenderTarget(Renderer* renderer)
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
    TextureRenderView->Release();
}

void Texture2D::LoadTextureFromFile()
{
    TextureData = stbi_load(FilePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, 4);
    RowPitch = TexDimensionsW * pixelComponent;
}




TextureCube::TextureCube(std::string parfilepath)
{
    PartialFilePath = parfilepath;
}

TextureCube::~TextureCube()
{
    ReleaseTexture();
}

bool TextureCube::CreateTextureFromFile(Renderer* renderer)
{
    HRESULT hr;
    LoadTextureFromFile();
   
    TextureDesc.Width = TexDimensionsW;
    TextureDesc.Height = TexDimensionsH;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 6;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
        cubemapSubResourceData[i].SysMemPitch = RowPitch;
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
        free(TextureData[i]);
    }
    //free(TextureData);

    return true;
}

void TextureCube::BindTexture(Renderer* renderer, int bindslot)
{
    renderer->gfxContext->PSSetShaderResources(bindslot, 1, TextureShaderView.GetAddressOf());
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
        std::string completePath = PartialFilePath + std::to_string(i) + ".jpg"; //probably change to png
        TextureData[i] = stbi_load(completePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, 4);
    }
    RowPitch = TexDimensionsW * 4;
}
