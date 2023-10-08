#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Vector.h"
#include <string>

class Renderer;
class Window;
class VertexShader;
class PixelShader;
class Texture2D
{
public:
	Texture2D();
	Texture2D(std::string filepath, int c = 4, bool hdr = false);
	~Texture2D();
	bool CreateTextureFromFile(Renderer* renderer, int bitsperpixel = 8, bool genMips = true, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	bool CreateRenderTexture(Renderer* renderer, int w, int h, int bitsperpixel = 8, int c = 4, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	void BindTexture(Renderer* renderer, int bindslot) const;
	void BindAsRenderTarget(Renderer* renderer) const;
	void ReleaseTexture();
private:
	void LoadTextureFromFile();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureShaderView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> TextureRenderView;
	D3D11_TEXTURE2D_DESC TextureDesc;
	int component;
	int RowPitch;
	int pixelComponent;
	int TexDimensionsW;
	int TexDimensionsH;
	unsigned char* TextureData;
	float* TextureDataF;
	std::string FilePath;
	bool IsRenderTexture;
	bool HDR;
	//D3D11_SUBRESOURCE_DATA TextureSRD;
};

class TextureCube
{
public:
	//right = 0, left = 1, up = 2, down = 3, front = 4, back = 5
	TextureCube();
	TextureCube(std::string parfilepath, int c = 4, bool hdr = false);
	~TextureCube();
	bool CreateTextureFromFile(Renderer* renderer, int bitsperpixel = 8, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	bool CreateCubeMapRenderTexture(Renderer* renderer, int w, int h, int bitsperpixel = 8, int c = 4, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,  bool mip = false, int miplevels = 1);
	
	void BindTexture(Renderer* renderer, int bindslot) const;
	void BindAsRenderTarget(Renderer* renderer, int face) const;
	void RenderHDRIToCubeMap(Renderer* renderer, Window* wndw, Texture2D const &HDRI);
	void RenderPrefilteredCubeMap(Renderer* renderer, Window* wndw, TextureCube const &cubemap, VertexShader const &vertShader, PixelShader const &pixShader);
	void RenderPrefilteredCubeWithMips(Renderer* renderer, Window* wndw, TextureCube const& cubemap, VertexShader const& vertShader, PixelShader const& pixShader);
	void ReleaseTexture();
private:
	void LoadTextureFromFile();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureShaderView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> TextureRenderView[6];
	D3D11_TEXTURE2D_DESC TextureDesc;
	int component;
	int RowPitch;
	int pixelComponent;
	int TexDimensionsW;
	int TexDimensionsH;
	bool IsRenderTexture;
	bool HDR;
	unsigned char* TextureData[6];
	float* TextureDataF[6];
	std::string PartialFilePath;
};