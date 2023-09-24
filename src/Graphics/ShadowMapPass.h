#pragma once

#include "Model.h"
#include "Direct3D11\Direct3D11VertexShader.h"
#include "Direct3D11\Direct3D11PixelShader.h"
#include <memory>


class ShadowMapPass
{
public:
	ShadowMapPass(Direct3D11Renderer& d3dRenderer);

	void AddToPass(std::shared_ptr<Model> model);

	void Execute(Direct3D11Renderer& d3dRenderer);
	void SetResources(Direct3D11Renderer& d3dRenderer);
private:
	std::vector<std::shared_ptr<Model>> renderQueue;
	std::shared_ptr<Direct3D11VertexShader> pVertexShader;
	std::shared_ptr<Direct3D11PixelShader> pPixelShader;
};