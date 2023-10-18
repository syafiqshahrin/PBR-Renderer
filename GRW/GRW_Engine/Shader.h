#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Debug.h"
#include <vector>
#include "Vector.h"


class Renderer;




class VertexShader
{
public:
	VertexShader();
	VertexShader(std::string FilePath);
	~VertexShader();
	void CreateShader(Renderer* renderer);
	void BindShader(Renderer* renderer) const;
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
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

};


class PixelShader
{
public:
	PixelShader();
	PixelShader(std::string FilePath);
	~PixelShader();
	void CreateShader(Renderer* renderer);
	void BindShader(Renderer* renderer) const;
private:
	std::string ShaderFilePath;
	void LoadShaderFromFile();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixShader;
	Microsoft::WRL::ComPtr <ID3DBlob> pshaderBlob;
};

enum ShaderParamType {
	SCALAR = 0,
	VECTOR
};

struct ShaderTextureParam
{
	std::string paramName;
	std::string defaultTexture;
	UINT bindSlot;
};

struct ShaderConstantParam
{
	std::string paramName;
	ShaderParamType paramType;
	UINT paramIndex;
	UINT offset;
	float defaultScalarValue;
	Vector4 defaultVectorValue;
};

struct MaterialShader
{
	std::string MaterialShaderName;
	std::string VSName;
	std::string PSName;

	std::vector<ShaderTextureParam> TextureParameters;
	std::vector<ShaderConstantParam> ShaderParameters;
};