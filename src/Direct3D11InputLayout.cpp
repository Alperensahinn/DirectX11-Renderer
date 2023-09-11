#include "Direct3D11InputLayout.h"

Direct3D11InputLayout::Direct3D11InputLayout(Direct3D11Renderer& d3dRenderer, std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc, ID3DBlob* blob)
{
	d3dRenderer.GetDevice()->CreateInputLayout(elementDesc.data(), elementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &pInputLayout);
}

void Direct3D11InputLayout::Bind(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.GetImmediateContext()->IASetInputLayout(pInputLayout.Get());
}