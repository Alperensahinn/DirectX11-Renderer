#pragma once

#include "Material.h"
#include <memory>

//TODO: hashmap based resource management to prevent duplication

class MaterialFactory
{
public:
	MaterialFactory() = default;
	

	static std::shared_ptr<Material> CreateMaterial(Direct3D11Renderer& pd3dRenderer, std::string vertexShaderPath, std::string pixelShaderPath, std::string albedoTexturePath);
private:
};