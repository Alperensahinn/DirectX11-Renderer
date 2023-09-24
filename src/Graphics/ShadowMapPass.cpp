#include "ShadowMapPass.h"
#include "Direct3D11\Direct3D11ResourceMap.h"

ShadowMapPass::ShadowMapPass(Direct3D11Renderer& d3dRenderer)
{
	pVertexShader = d3dRenderer.GetResourceMap().GetD3D11VertexShader(d3dRenderer, "x64\\Debug\\VS_Shadow.cso");
	pPixelShader = d3dRenderer.GetResourceMap().GetD3D11PixelShader(d3dRenderer, "x64\\Debug\\PS_Shadow.cso");
}

void ShadowMapPass::AddToPass(std::shared_ptr<Model> model)
{
	for (int i = 0; i < renderQueue.size(); i++)
	{
		if(renderQueue[i] == model)
		{
			return;
		}
	}

	renderQueue.push_back(model);
}

void ShadowMapPass::Execute(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.drawMode = 0;
	d3dRenderer.ShadowPass();

	for(int i = 0; i < renderQueue.size(); i++)
	{
		renderQueue[i].get()->Draw(d3dRenderer, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	}
}

void ShadowMapPass::SetResources(Direct3D11Renderer& d3dRenderer)
{
	pVertexShader.get()->Bind(d3dRenderer);
	d3dRenderer.GetImmediateContext()->PSSetShader(NULL, NULL, 0u);
}


