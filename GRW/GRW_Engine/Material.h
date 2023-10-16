#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <map>
#include "CBuffer.h"
#include "Vector.h"

class Texture2D;
class TextureCube;
class VertexShader;
class PixelShader;

struct MaterialBuffer
{
	float ParamList[64]; //holds up to 16 vec4 or up to 64 floats
};


struct TexParam
{
	std::string paramName;
	Texture2D* texture;
	UINT bindSlot;
	bool IsRenderTexture;
};


enum ShaderParamType {
	SCALAR = 0,
	VECTOR
};

struct ShaderParam
{
	ShaderParamType type;
	std::string paramName;
	UINT offset;
	float valueF;
	Vector4 valueV;
};

enum BlendMode
{
	Opaque = 0,
	Transparent,
	Additive,
	Multiply
};
enum CullMode
{
	BACK = 0,
	FRONT,
	TWO_SIDED
};

enum FillMode
{
	SOLID = 0,
	WIRE
};

class MaterialAsset
{
public:
	MaterialAsset();
	~MaterialAsset();

	void CreateMaterial(Renderer* renderer, std::string name, VertexShader* vs, PixelShader* ps, std::vector<ShaderParam> const& param, std::vector<TexParam> const &texParam, BlendMode blend, CullMode cull, FillMode fill);

	void SetScalarParam(std::string paramName, float const& val);
	void SetVectorParam(std::string paramName, Vector4 const& val);

	float GetScalarParam(std::string paramName);
	Vector4 GetVectorParam(std::string paramName);

	void SetCullMode(CullMode cMode);
	void SetFillMode(FillMode fMode);
	void SetBlendMode(BlendMode bMode);

	void SetTexture(std::string TexName, Texture2D* tex, UINT bindslot, bool IsRT);
	void GetTextureParamNames(std::vector<std::string> &Textures);

	void BindMaterial(Renderer* renderer);
	void UpdateMaterial(Renderer* renderer);
private:
	HRESULT hr;
	//controls fill mode and cull mode and msaa mode - only expose these
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	//controls blendmode - alpha blend (transparent), opaque, masked/alpha clip
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::map<std::string, UINT> ParamIDs; //param name as key and offset as value
	std::map<std::string, TexParam> TextureParams;
	std::map<std::string, TextureCube*> TextureCubeParams;

	CBuffer<MaterialBuffer> ParameterBuffer;

	VertexShader* VShader;
	PixelShader* PShader;
	std::string MaterialName;

	BlendMode blendMode;
	CullMode cullMode;
	FillMode fillMode;

	bool BlendDirty;
	bool RasterizerDirty;
	bool BufferDirty;

	void CreateBuffer(Renderer* renderer);
	void CreateBlendState(Renderer* renderer);
	void CreateRasterizerState(Renderer* renderer);

	void BindShaders(Renderer* renderer);
	void BindTextures(Renderer* renderer);
	void BindBuffer(Renderer* renderer);
	void BindRasterizerState(Renderer * renderer);
	void BindBlendState(Renderer * renderer);
	
	void UpdateBuffer(Renderer* renderer);
	void UpdateBlendState(Renderer* renderer);
	void UpdateRasterizerState(Renderer* renderer);
	

	void InitialiseParamBuffer(std::vector<ShaderParam> const &param);
	void InitialiseTextureParam(std::vector<TexParam> const &texParam);

};

struct MaterialAssetData
{
	std::string Name;
	std::vector<TexParam> textureParams;
	std::vector<ShaderParam> parameters;
	BlendMode blend;
	CullMode cull;
	FillMode fill;
	VertexShader* vs;
	PixelShader* ps;
};