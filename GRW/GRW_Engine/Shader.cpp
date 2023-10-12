#include "Shader.h"
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <comdef.h>
#include "Debug.h"
#include "Renderer.h"

VertexShader::VertexShader()
{
}

VertexShader::VertexShader(std::string FilePath)
{
	ShaderFilePath = FilePath;
}

VertexShader::~VertexShader()
{

}

void VertexShader::CreateShader(Renderer* renderer)
{
    LoadShaderFromFile();
    HRESULT hr = renderer->gfxDevice->CreateVertexShader(vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), nullptr, &VertShader);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creatiing input layout" << errorText);

        //return -1;
    }
    renderer->gfxContext->VSSetShader(VertShader.Get(), nullptr, 0u);

    hr = renderer->gfxDevice->CreateInputLayout(inLayoutDesc, 5, vshaderBlob->GetBufferPointer(), vshaderBlob->GetBufferSize(), &InputLayoutHandle);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();
        DEBUG("Failed creatiing input layout" << errorText);

        //return -1;
    }
    renderer->gfxContext->IASetInputLayout(InputLayoutHandle.Get());
    //vshaderBlob->Release();
}

void VertexShader::BindShader(Renderer* renderer) const
{
    renderer->gfxContext->VSSetShader(VertShader.Get(), nullptr, 0u);
    renderer->gfxContext->IASetInputLayout(InputLayoutHandle.Get());
}

void VertexShader::LoadShaderFromFile()
{
    std::wstring path = std::wstring(ShaderFilePath.begin(), ShaderFilePath.end());
    HRESULT hr = D3DReadFileToBlob(path.c_str(), &vshaderBlob);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to load shader cso file :" << errorText);
    }
}

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(std::string FilePath)
{
    ShaderFilePath = FilePath;
}

PixelShader::~PixelShader()
{

}

void PixelShader::CreateShader(Renderer* renderer)
{
    LoadShaderFromFile();

    HRESULT hr = renderer->gfxDevice->CreatePixelShader(pshaderBlob->GetBufferPointer(), pshaderBlob->GetBufferSize(), nullptr, &PixShader);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to load shader cso file :" << errorText);
    }
    //pshaderBlob->Release();
}

void PixelShader::BindShader(Renderer* renderer) const
{
    renderer->gfxContext->PSSetShader(PixShader.Get(), nullptr, 0u);
}

void PixelShader::LoadShaderFromFile()
{
    std::wstring path = std::wstring(ShaderFilePath.begin(), ShaderFilePath.end());
    HRESULT hr = D3DReadFileToBlob(path.c_str(), &pshaderBlob);
    if (FAILED(hr))
    {
        _com_error error(hr);
        LPCTSTR errorText = error.ErrorMessage();

        DEBUG("Failed to load shader cso file :" << errorText);
    }
}
