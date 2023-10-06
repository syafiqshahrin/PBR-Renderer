#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Vector.h"
#include <string>

class Renderer;
class Window;

class Texture2D
{
public:
	Texture2D();
	Texture2D(std::string filepath);
	~Texture2D();
	bool CreateTextureFromFile(Renderer* renderer, bool genMips = true, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	bool CreateRenderTexture(Renderer* renderer, int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	void BindTexture(Renderer* renderer, int bindslot) const;
	void BindAsRenderTarget(Renderer* renderer) const;
	void ReleaseTexture();
private:
	void LoadTextureFromFile();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureShaderView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> TextureRenderView;
	D3D11_TEXTURE2D_DESC TextureDesc;
	int RowPitch;
	int pixelComponent;
	int TexDimensionsW;
	int TexDimensionsH;
	unsigned char* TextureData;
	std::string FilePath;
	bool IsRenderTexture;
	//D3D11_SUBRESOURCE_DATA TextureSRD;
};

class TextureCube
{
public:
	//right = 0, left = 1, up = 2, down = 3, front = 4, back = 5
	TextureCube();
	TextureCube(std::string parfilepath);
	~TextureCube();
	bool CreateTextureFromFile(Renderer* renderer);
	bool CreateCubeMapRenderTexture(Renderer* renderer, int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	void BindTexture(Renderer* renderer, int bindslot);
	void BindAsRenderTarget(Renderer* renderer, int face);
	void RenderHDRIToCubeMap(Renderer* renderer, Window* wndw, Texture2D const &HDRI);
	void ReleaseTexture();
private:
	void LoadTextureFromFile();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureShaderView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> TextureRenderView[6];
	D3D11_TEXTURE2D_DESC TextureDesc;

	int RowPitch;
	int pixelComponent;
	int TexDimensionsW;
	int TexDimensionsH;
	bool IsRenderTexture;
	unsigned char* TextureData[6];
	std::string PartialFilePath;
};