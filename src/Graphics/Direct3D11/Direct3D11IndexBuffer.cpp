#include "Direct3D11IndexBuffer.h"
#include "DirectX11Error.h"

Direct3D11IndexBuffer::Direct3D11IndexBuffer(Direct3D11Renderer& d3dRenderer, std::vector<unsigned int>& indices)
{
	CheckerToken chk = {};

	D3D11_BUFFER_DESC ibdesc = {};
	ibdesc.ByteWidth = sizeof(unsigned int) * indices.size();
	ibdesc.Usage = D3D11_USAGE_DEFAULT;
	ibdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibdesc.CPUAccessFlags = 0u;
	ibdesc.MiscFlags = 0u;
	ibdesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA ibdata = {};
	ibdata.pSysMem = indices.data();

	d3dRenderer.GetDevice()->CreateBuffer(&ibdesc, &ibdata, &pIndexBuffer) >>chk;
}

void Direct3D11IndexBuffer::Bind(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.GetImmediateContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}
