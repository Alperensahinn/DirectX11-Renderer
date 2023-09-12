#include "Material.h"

Material::Material(MATERIAL_DESC& materialDescription)
{
	pAlbedoTexture = materialDescription.AlbedoTexture;

	pVertexShader = materialDescription.VertexShader;
	pPixelShader = materialDescription.PixelShader;
}

void Material::Bind(Direct3D11Renderer& d3dRenderer)
{
	pAlbedoTexture.get()->Bind(d3dRenderer);

	pVertexShader.get()->Bind(d3dRenderer);
	pPixelShader.get()->Bind(d3dRenderer);
}
