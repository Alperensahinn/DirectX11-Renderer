#pragma once

#include "Direct3D11Renderer.h"
#include "GraphicsError.h"
#include <wrl.h>

template<typename T>
class Direct3D11ConstantBuffer
{
public:
	enum ConstantBufferType
	{
		VertexShaderConstantBuffer,
		PixelShaderConstantBuffer
	};

	Direct3D11ConstantBuffer(Direct3D11Renderer d3dRenderer, std::unique_ptr<T>& bufferData, ConstantBufferType type) : type(type)
	{
		CheckerToken chk = {};

		D3D11_BUFFER_DESC cbdesc = {};
		cbdesc.ByteWidth = sizeof(T);
		cbdesc.Usage = D3D11_USAGE_DYNAMIC;
		cbdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbdesc.MiscFlags = 0u;
		cbdesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA cbdata = {};
		cbdata.pSysMem = bufferData.get();

		d3dRenderer.GetDevice()->CreateBuffer(&cbdesc, &cbdata, &pConstantBuffer) >> chk;
	}

	void UpdateData(Direct3D11Renderer d3dRenderer, std::unique_ptr<T>& bufferData)
	{
		CheckerToken chk = {};

		D3D11_MAPPED_SUBRESOURCE ms = {};

		d3dRenderer.GetImmediateContext()->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms) >> chk;
		memcpy(ms.pData, bufferData.get(), sizeof(T));
		d3dRenderer.GetImmediateContext()->Unmap(pConstantBuffer.Get(), 0u);
	}

	void Bind(Direct3D11Renderer d3dRenderer)
	{
		if (type == VertexShaderConstantBuffer)
		{
			d3dRenderer.GetImmediateContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
		else if(type == PixelShaderConstantBuffer)
		{
			d3dRenderer.GetImmediateContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	ConstantBufferType type;
};