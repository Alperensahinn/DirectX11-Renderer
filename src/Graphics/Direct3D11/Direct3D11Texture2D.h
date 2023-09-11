#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>

class Direct3D11Texture2D
{
public:
	Direct3D11Texture2D(Direct3D11Renderer& d3dRenderer, std::string filePath, unsigned int slot);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture2DResourceView;
	unsigned int slot;
};