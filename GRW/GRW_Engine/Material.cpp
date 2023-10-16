#include "Material.h"
#include "Renderer.h"
#include "Debug.h"
#include "Texture.h"
#include "Shader.h"


void MaterialAsset::CreateBuffer(Renderer* renderer)
{
	ParameterBuffer.CreateBuffer(renderer);
}

void MaterialAsset::CreateBlendState(Renderer* renderer)
{
	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = false;
	BlendStateDesc.IndependentBlendEnable = false;
	BlendStateDesc.RenderTarget[0].BlendEnable = true;
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	switch(blendMode)
	{
		case BlendMode::Opaque:
			BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case BlendMode::Transparent:
			BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case BlendMode::Additive:
			BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case BlendMode::Multiply:
			BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
			BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
	}

	renderer->gfxDevice->CreateBlendState(&BlendStateDesc, &blendState);
}

void MaterialAsset::CreateRasterizerState(Renderer* renderer)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias = false;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	switch(cullMode)
	{
		case CullMode::BACK:
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			break;
		case CullMode::FRONT:
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			break;
		case CullMode::TWO_SIDED:
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			break;
	}

	switch (fillMode)
	{
	case FillMode::SOLID:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		break;
	case FillMode::WIRE:
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		break;
	}


	renderer->gfxDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
}

void MaterialAsset::BindShaders(Renderer* renderer)
{
	VShader->BindShader(renderer);
	PShader->BindShader(renderer);
}

void MaterialAsset::BindTextures(Renderer* renderer)
{
	for (std::map<std::string, TexParam>::iterator it = TextureParams.begin(); it != TextureParams.end(); it++)
	{
		it->second.texture->BindTexture(renderer, it->second.bindSlot);
	}
}

void MaterialAsset::BindBuffer(Renderer* renderer)
{
	ParameterBuffer.BindBuffer(renderer, 2);
}

void MaterialAsset::BindRasterizerState(Renderer* renderer)
{
	renderer->gfxContext->RSSetState(rasterizerState.Get());
}

void MaterialAsset::BindBlendState(Renderer* renderer)
{
	//float factor[4] = { 1,1,1,1 };
	renderer->gfxContext->OMSetBlendState(blendState.Get(), NULL, 0xffffffff);
}

void MaterialAsset::UpdateBuffer(Renderer* renderer)
{	
	ParameterBuffer.UpdateBuffer(renderer);
	BufferDirty = false;
}

void MaterialAsset::UpdateBlendState(Renderer* renderer)
{
	CreateBlendState(renderer);
	BlendDirty = false;
}

void MaterialAsset::UpdateRasterizerState(Renderer* renderer)
{
	CreateRasterizerState(renderer);
	RasterizerDirty = false;
}

void MaterialAsset::InitialiseParamBuffer(std::vector<ShaderParam> const& param)
{
	for (ShaderParam sp : param)
	{
		ParamIDs.insert({sp.paramName, sp.offset});
		if (sp.type == ShaderParamType::SCALAR)
		{
			ParameterBuffer.BufferData.ParamList[sp.offset] = sp.valueF;
		}
		else if (sp.type == ShaderParamType::VECTOR)
		{
			ParameterBuffer.BufferData.ParamList[sp.offset] = sp.valueV.x;
			ParameterBuffer.BufferData.ParamList[sp.offset + 1] = sp.valueV.y;
			ParameterBuffer.BufferData.ParamList[sp.offset + 2] = sp.valueV.z;
			ParameterBuffer.BufferData.ParamList[sp.offset + 3] = sp.valueV.w;
		}
	}
}

void MaterialAsset::InitialiseTextureParam(std::vector<TexParam> const &texParam)
{
	for (TexParam tp : texParam)
	{
		TextureParams.insert({tp.paramName, tp});
	}
}

void MaterialAsset::BindMaterial(Renderer* renderer)
{
	BindShaders(renderer);
	BindBlendState(renderer);
	BindRasterizerState(renderer);
	BindBuffer(renderer);
	BindTextures(renderer);
}

void MaterialAsset::UpdateMaterial(Renderer* renderer)
{
	if (BufferDirty)
	{
		UpdateBuffer(renderer);
	}

	if (RasterizerDirty)
	{
		UpdateRasterizerState(renderer);
	}

	if (BlendDirty)
	{
		UpdateBlendState(renderer);
	}
}

MaterialAsset::MaterialAsset()
{
	BlendDirty = false;
	BufferDirty = false;
	RasterizerDirty = false;
}

MaterialAsset::~MaterialAsset()
{

}

void MaterialAsset::CreateMaterial(Renderer* renderer, std::string name, VertexShader* vs, PixelShader* ps, std::vector<ShaderParam> const& param, std::vector<TexParam> const& texParam, BlendMode blend, CullMode cull, FillMode fill)
{
	VShader = vs;
	PShader = ps;
	MaterialName = name;

	blendMode = blend;
	cullMode = cull;
	fillMode = fill;

	InitialiseParamBuffer(param);
	InitialiseTextureParam(texParam);

	CreateBuffer(renderer);
	CreateBlendState(renderer);
	CreateRasterizerState(renderer);
}

void MaterialAsset::SetScalarParam(std::string paramName, float const& val)
{
	BufferDirty = true;
	UINT offset = ParamIDs[paramName];
	ParameterBuffer.BufferData.ParamList[offset] = val;
}

void MaterialAsset::SetVectorParam(std::string paramName, Vector4 const& val)
{
	BufferDirty = true;
	UINT offset = ParamIDs[paramName];
	ParameterBuffer.BufferData.ParamList[offset] = val.x;
	ParameterBuffer.BufferData.ParamList[offset + 1] = val.y;
	ParameterBuffer.BufferData.ParamList[offset + 2] = val.z;
	ParameterBuffer.BufferData.ParamList[offset + 3] = val.w;
}

float MaterialAsset::GetScalarParam(std::string paramName)
{
	UINT offset = ParamIDs[paramName];
	return ParameterBuffer.BufferData.ParamList[offset];
}

Vector4 MaterialAsset::GetVectorParam(std::string paramName)
{
	UINT offset = ParamIDs[paramName];
	Vector4 value;
	value.x = ParameterBuffer.BufferData.ParamList[offset];
	value.y = ParameterBuffer.BufferData.ParamList[offset + 1];
	value.z = ParameterBuffer.BufferData.ParamList[offset + 2];
	value.w = ParameterBuffer.BufferData.ParamList[offset + 3];
	return value;
}

void MaterialAsset::SetCullMode(CullMode cMode)
{
	RasterizerDirty = true;
	cullMode = cMode;
}

void MaterialAsset::SetFillMode(FillMode fMode)
{
	RasterizerDirty = true;
	fillMode = fMode;
}

void MaterialAsset::SetBlendMode(BlendMode bMode)
{
	BlendDirty = true;
	blendMode = bMode;
}

void MaterialAsset::SetTexture(std::string TexParamName, Texture2D* tex,  UINT bindslot, bool IsRT)
{
	TexParam param;
	param.paramName = TexParamName;
	param.texture = tex;
	param.bindSlot = bindslot;
	param.IsRenderTexture = IsRT;

	TextureParams[TexParamName] = param;
}

void MaterialAsset::GetTextureParamNames(std::vector<std::string>& Textures)
{
	for (std::map<std::string, TexParam>::iterator it = TextureParams.begin(); it != TextureParams.end(); it++)
	{
		Textures.push_back(it->first);
	}
}
