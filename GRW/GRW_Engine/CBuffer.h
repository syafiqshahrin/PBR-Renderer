#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Debug.h"
#include <comdef.h>
#include "Renderer.h"

class Renderer;

template <typename T> class CBuffer
{
public:
	T BufferData;
	CBuffer();
	void UpdateBuffer(Renderer* renderer);
	void BindBuffer(Renderer* renderer, int bindslot);
	void CreateBuffer(Renderer* renderer);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};

template<typename T>
inline CBuffer<T>::CBuffer()
{

}

template<typename T>
inline void CBuffer<T>::UpdateBuffer(Renderer* renderer)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    renderer->gfxContext->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &BufferData, sizeof(BufferData));
    renderer->gfxContext->Unmap(ConstantBuffer.Get(), 0);
}

template<typename T>
inline void CBuffer<T>::BindBuffer(Renderer* renderer, int bindslot)
{
    renderer->gfxContext->VSSetConstantBuffers(bindslot, 1, ConstantBuffer.GetAddressOf());
    renderer->gfxContext->PSSetConstantBuffers(bindslot, 1, ConstantBuffer.GetAddressOf());
}

template<typename T>
inline void CBuffer<T>::CreateBuffer(Renderer* renderer)
{
    HRESULT hr;
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof(BufferData);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &BufferData;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    hr = renderer->gfxDevice->CreateBuffer(&cbDesc, &data, &ConstantBuffer);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to create cbuffer:" << errorText);
    }
}
