#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>
#include <string>

class Direct3D11PixelShader
{
public:
	Direct3D11PixelShader(Direct3D11Renderer& d3dRenderer, std::string path);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};