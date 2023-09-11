#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>

class Direct3D11SamplerState
{
public:
	Direct3D11SamplerState(Direct3D11Renderer& d3dRenderer);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
};