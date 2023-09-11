#include "Direct3D11Texture2D.h"
#include "DirectX11Error.h"
#include "..\..\3rdparty\stb_image.h"

Direct3D11Texture2D::Direct3D11Texture2D(Direct3D11Renderer& d3dRenderer, std::string filePath, unsigned int slot) : slot(slot)
{	
	CheckerToken chk = {};

	int width;
	int height;
	int nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;

	D3D11_TEXTURE2D_DESC tdesc = {};
	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = 0u;
	tdesc.ArraySize = 1u;
	tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tdesc.SampleDesc.Count = 1u;
	tdesc.SampleDesc.Quality = 0u;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tdesc.CPUAccessFlags = 0u;
	tdesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	d3dRenderer.GetDevice()->CreateTexture2D(&tdesc, NULL, &pTexture2D) >> chk;
	d3dRenderer.GetImmediateContext()->UpdateSubresource(pTexture2D.Get(), 0u, NULL, data, sizeof(int) * width, 0u);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = {};
	rvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rvdesc.Texture2D.MostDetailedMip = 0;
	rvdesc.Texture2D.MipLevels = -1;

	d3dRenderer.GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &rvdesc, &pTexture2DResourceView) >> chk;
	d3dRenderer.GetImmediateContext()->GenerateMips(pTexture2DResourceView.Get());

	stbi_image_free(data);
}

void Direct3D11Texture2D::Bind(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.GetImmediateContext()->PSSetShaderResources(slot, 1u, pTexture2DResourceView.GetAddressOf());
}
