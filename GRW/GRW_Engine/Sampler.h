#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class Renderer;

class Sampler
{
public:
	Sampler();
	~Sampler();
	void BindSampler(UINT bindslot, Renderer* renderer);
	bool CreateSampler(Renderer* renderer, D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_MODE w = D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
	D3D11_SAMPLER_DESC samplerDesc;
};