#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <map>

class Texture2D;
class TextureCube;

class Material
{
public:
	Material();
	~Material();
	//need ref to shader
	//refs for textures to bind
	//per material cbuffer - uses MaterialCbuffer struct
		//string map - shader parameter ID - parameter name as key and array offset as value
		//One map for each input type - Float, Vector4(used for vec2, 3, 4)
		//Material Mimp file will store string map and offset value for each map
		//Updating cbuffer values using SetFloat(string key, float)/SetVector(string key, vec4)
		//Getitng buffer values using GetFloat(string key)/GetVector(string key)
	//sets rasterizer state
	//sets blending state


private:
	HRESULT hr;
	//controls fill mode and cull mode and msaa mode - only expose these
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	//controls blendmode - alpha blend (transparent), opaque, masked/alpha clip
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::map<std::string, int> FloatParamIDs;
	std::map<std::string, int> Vector4ParamIDs;
	std::map<std::string, Texture2D*> TextureParams;
	std::map<std::string, TextureCube*> TextureCubeParams;
};