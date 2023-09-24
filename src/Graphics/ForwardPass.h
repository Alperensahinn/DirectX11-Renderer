#pragma once

#include "Model.h"
#include "Direct3D11\Direct3D11Renderer.h"

class ForwardPass
{
public:
	ForwardPass() = default;

	void AddToPass(std::shared_ptr<Model> model);

	void Execute(Direct3D11Renderer& d3dRenderer);
	void SetResources(Direct3D11Renderer& d3dRenderer);
private:
	std::vector<std::shared_ptr<Model>> renderQueue;
};