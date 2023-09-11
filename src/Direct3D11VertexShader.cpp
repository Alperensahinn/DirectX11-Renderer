#include "Direct3D11VertexShader.h"
#include "GraphicsError.h"

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

Direct3D11VertexShader::Direct3D11VertexShader(Direct3D11Renderer& d3dRenderer, std::string path)
{
	CheckerToken chk = {};

	std::wstring stemp = std::wstring(path.begin(), path.end());
	LPCWSTR sw = stemp.c_str();

	D3DReadFileToBlob(sw, &pBlob) >> chk;

	d3dRenderer.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader) >> chk;
}

void Direct3D11VertexShader::Bind(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.GetImmediateContext()->VSSetShader(pVertexShader.Get(), NULL, 0u);
}

ID3DBlob* Direct3D11VertexShader::GetBlob()
{
	return pBlob.Get();
}
