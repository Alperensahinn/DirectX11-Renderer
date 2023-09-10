#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>

class Direct3D11VertexShader
{
public:
	Direct3D11VertexShader(Direct3D11Renderer& pd3dRenderer, std::string path);

	void Bind(Direct3D11Renderer& pd3dRenderer);
	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};