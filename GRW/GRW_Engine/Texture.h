#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Vector.h"
#include <string>

class Renderer;

class Texture2D
{
public:
	Texture2D(std::string filepath);
	~Texture2D();
	bool CreateTexture(Renderer* renderer);
	void BindTexture(Renderer* renderer, int bindslot);
	void ReleaseTexture();
private:
	void LoadTextureFromFile();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureShaderView;
	D3D11_TEXTURE2D_DESC TextureDesc;
	int RowPitch;
	int pixelComponent;
	int TexDimensionsW;
	int TexDimensionsH;
	unsigned char* TextureData;
	std::string FilePath;
	//D3D11_SUBRESOURCE_DATA TextureSRD;
};