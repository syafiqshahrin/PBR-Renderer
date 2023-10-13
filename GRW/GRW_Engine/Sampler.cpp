#include "Sampler.h"
#include "Renderer.h"
#include <comdef.h>
#include "Debug.h"


Sampler::Sampler()
{

}

Sampler::~Sampler()
{

}

void Sampler::BindSampler(UINT bindslot, Renderer* renderer)
{
    renderer->gfxContext->PSSetSamplers(bindslot, 1, SamplerState.GetAddressOf());
}

bool Sampler::CreateSampler(Renderer* renderer, D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w, D3D11_FILTER filter)
{
    
    samplerDesc.Filter = filter;
    samplerDesc.AddressU = u;
    samplerDesc.AddressV = v;
    samplerDesc.AddressW = w;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = FLT_MAX;
    samplerDesc.MinLOD = 0.0f;

    HRESULT hr = renderer->gfxDevice->CreateSamplerState(&samplerDesc, &SamplerState);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creating sampler state - " << errorText);

        return false;
    }
	return true;
}
