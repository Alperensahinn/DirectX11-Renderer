#include "MaterialFactory.h"

std::shared_ptr<Material> MaterialFactory::CreateMaterial(Direct3D11Renderer& pd3dRenderer, std::string vertexShaderPath, std::string pixelShaderPath, std::string albedoTexturePath)
{
	std::shared_ptr<Direct3D11VertexShader> pVertexShader = std::make_unique<Direct3D11VertexShader>(pd3dRenderer, vertexShaderPath);
	std::shared_ptr<Direct3D11PixelShader> pPixelShader = std::make_unique<Direct3D11PixelShader>(pd3dRenderer, pixelShaderPath);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	std::shared_ptr<Direct3D11InputLayout> pInputLayout = std::make_shared<Direct3D11InputLayout>(pd3dRenderer, ied, pVertexShader.get()->GetBlob());

	std::shared_ptr<Direct3D11Texture2D> pTexture = std::make_shared<Direct3D11Texture2D>(pd3dRenderer, albedoTexturePath, 0u);

	std::shared_ptr<Direct3D11SamplerState> pSamplerState = std::make_shared<Direct3D11SamplerState>(pd3dRenderer);

	Material::MATERIAL_DESC mdesc = {};
	mdesc.AlbedoTexture = pTexture;
	mdesc.VertexShader = pVertexShader;
	mdesc.PixelShader = pPixelShader;
	mdesc.InputLayout = pInputLayout;
	mdesc.SamplerState = pSamplerState;

	std::shared_ptr<Material> pMaterial = std::make_shared<Material>(mdesc);

    return pMaterial;
}
