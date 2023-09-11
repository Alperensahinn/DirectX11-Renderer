#include "Direct3D11SamplerState.h"
#include "DirectX11Error.h"

Direct3D11SamplerState::Direct3D11SamplerState(Direct3D11Renderer& d3dRenderer)
{
	CheckerToken chk = {};

	D3D11_SAMPLER_DESC sdesc = {};
	sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.MipLODBias = 0u;
	sdesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sdesc.MinLOD = 0.0f;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dRenderer.GetDevice()->CreateSamplerState(&sdesc, &pSamplerState) >> chk;
}

void Direct3D11SamplerState::Bind(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.GetImmediateContext()->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
}
