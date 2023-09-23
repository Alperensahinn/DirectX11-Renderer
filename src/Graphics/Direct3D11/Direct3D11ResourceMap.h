#pragma once

#include "Direct3D11VertexShader.h"
#include "Direct3D11PixelShader.h"

#include <unordered_map>
#include <memory>

class Direct3D11ResourceMap
{
public:
	std::shared_ptr<Direct3D11VertexShader> GetD3D11VertexShader(Direct3D11Renderer& d3dRenderer, std::string path);
	std::shared_ptr<Direct3D11PixelShader> GetD3D11PixelShader(Direct3D11Renderer& d3dRenderer, std::string path);

private:
	std::unordered_map<std::string, std::shared_ptr<Direct3D11VertexShader>> d3d11VertexShaderMap;
	std::unordered_map<std::string, std::shared_ptr<Direct3D11PixelShader>> d3d11PixelShaderMap;
};
