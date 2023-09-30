#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include "Renderer.h"
#include "Debug.h"
#include <comdef.h>

Texture2D::Texture2D(std::string filepath)
{
	FilePath = filepath;

}

Texture2D::~Texture2D()
{
    ReleaseTexture();
}

bool Texture2D::CreateTexture(Renderer* renderer)
{
    LoadTextureFromFile();
    TextureDesc.Width = TexDimensionsW;
    TextureDesc.Height = TexDimensionsH;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.CPUAccessFlags = 0;


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

    free(TextureData);

	return true;
}

void Texture2D::BindTexture(Renderer* renderer, int bindslot)
{
    renderer->gfxContext->PSSetShaderResources(bindslot, 1, TextureShaderView.GetAddressOf());
}

void Texture2D::ReleaseTexture()
{
	//if(TextureData != nullptr)
		//free(TextureData);
	Texture2DResource->Release();
    TextureShaderView->Release();
}

void Texture2D::LoadTextureFromFile()
{
    TextureData = stbi_load(FilePath.c_str(), &TexDimensionsW, &TexDimensionsH, &pixelComponent, 4);
    RowPitch = TexDimensionsW * pixelComponent;
}
