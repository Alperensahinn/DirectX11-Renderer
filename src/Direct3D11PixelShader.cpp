#include "Direct3D11PixelShader.h"
#include "GraphicsError.h"

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

Direct3D11PixelShader::Direct3D11PixelShader(Direct3D11Renderer& pd3dRenderer, std::string path)
{
	CheckerToken chk = {};

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	std::wstring stemp = std::wstring(path.begin(), path.end());
	LPCWSTR sw = stemp.c_str();

	D3DReadFileToBlob(sw, &pBlob) >> chk;

	pd3dRenderer.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader) >> chk;
}

void Direct3D11PixelShader::Bind(Direct3D11Renderer& pd3dRenderer)
{
	pd3dRenderer.GetImmediateContext()->PSSetShader(pPixelShader.Get(), NULL, 0u);
}
