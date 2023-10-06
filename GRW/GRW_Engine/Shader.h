#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Debug.h"

class Renderer;


class VertexShader
{
public:
	VertexShader(std::string FilePath);
	~VertexShader();
	void CreateShader(Renderer* renderer);
	void BindShader(Renderer* renderer);
private:
	std::string ShaderFilePath;
	void LoadShaderFromFile();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertShader;
	Microsoft::WRL::ComPtr <ID3DBlob> vshaderBlob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayoutHandle;
	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[6]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

};


class PixelShader
{
public:
	PixelShader(std::string FilePath);
	~PixelShader();
	void CreateShader(Renderer* renderer);
	void BindShader(Renderer* renderer);
private:
	std::string ShaderFilePath;
	void LoadShaderFromFile();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixShader;
	Microsoft::WRL::ComPtr <ID3DBlob> pshaderBlob;
};