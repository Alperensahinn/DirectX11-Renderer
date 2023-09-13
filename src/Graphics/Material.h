#pragma once
#include "Direct3D11\Direct3D11VertexShader.h"
#include "Direct3D11\Direct3D11PixelShader.h"
#include "Direct3D11\Direct3D11Texture2D.h"
#include "Direct3D11\Direct3D11InputLayout.h"
#include "Direct3D11\Direct3D11SamplerState.h"

#include <memory>

class Material
{
public:
	typedef struct MATERIAL_DESC
	{
		std::shared_ptr<Direct3D11VertexShader>	VertexShader;
		std::shared_ptr<Direct3D11PixelShader>	PixelShader;
		std::shared_ptr<Direct3D11SamplerState> SamplerState;
		std::shared_ptr<Direct3D11InputLayout> InputLayout;

		std::shared_ptr<Direct3D11Texture2D>	AlbedoTexture;
	};

public:
	Material(MATERIAL_DESC& pMaterialDescription);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	std::shared_ptr<Direct3D11Texture2D> pAlbedoTexture;


	std::shared_ptr<Direct3D11VertexShader> pVertexShader;
	std::shared_ptr<Direct3D11PixelShader> pPixelShader;
	std::shared_ptr<Direct3D11SamplerState> pSamplerState;
	std::shared_ptr<Direct3D11InputLayout> pInputLayout;
};