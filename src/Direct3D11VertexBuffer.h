#pragma once

#include "Direct3D11Renderer.h"
#include "GraphicsError.h"

#include <wrl.h>
#include <vector>

template<typename T>
class Direct3D11VertexBuffer
{
public:
	Direct3D11VertexBuffer(Direct3D11Renderer& pd3dRenderer, std::vector<T> vertices) : stride(sizeof(T))
	{
		CheckerToken chk = {};

		D3D11_BUFFER_DESC vbdesc = {};
		vbdesc.ByteWidth = sizeof(T) * vertices.size();
		vbdesc.Usage = D3D11_USAGE_DEFAULT;
		vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbdesc.CPUAccessFlags = 0u;
		vbdesc.MiscFlags = 0u;
		vbdesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA vsdata = {};
		vsdata.pSysMem = vertices.data();

		pd3dRenderer.GetDevice()->CreateBuffer(&vbdesc, &vsdata, &pVertexBuffer) >> chk;
	}

	void Bind(Direct3D11Renderer& pd3dRenderer)
	{
		unsigned int offset = 0u;
		pd3dRenderer.GetImmediateContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	unsigned int stride;
};