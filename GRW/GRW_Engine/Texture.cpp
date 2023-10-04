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
    TextureDesc.MipLevels = 0;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    TextureDesc.CPUAccessFlags = 0;

    //only need subresource if not creating texture w generated mips
    D3D11_SUBRESOURCE_DATA texSubResourceData;
    texSubResourceData.pSysMem = TextureData;
    texSubResourceData.SysMemPitch = RowPitch;
    texSubResourceData.SysMemSlicePitch = 0;

    HRESULT hr = renderer->gfxDevice->CreateTexture2D(&TextureDesc, nullptr, &Texture2DResource);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating texture2d resource - " << errorText);

        //return -1;
        return false;
    }

    //need this to generate mips
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
    //generate mips
    renderer->gfxContext->GenerateMips(TextureShaderView.Get());
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
