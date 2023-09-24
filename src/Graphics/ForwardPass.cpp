#include "ForwardPass.h"

void ForwardPass::AddToPass(std::shared_ptr<Model> model)
{
	for (int i = 0; i < renderQueue.size(); i++)
	{
		if (renderQueue[i] == model)
		{
			return;
		}
	}

	renderQueue.push_back(model);
}

void ForwardPass::Execute(Direct3D11Renderer& d3dRenderer)
{
	d3dRenderer.drawMode = 1;
	d3dRenderer.LambertianPass();

	for (int i = 0; i < renderQueue.size(); i++)
	{
		renderQueue[i].get()->Draw(d3dRenderer, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	}

	ID3D11ShaderResourceView* pSRV = NULL;
	d3dRenderer.GetImmediateContext()->PSSetShaderResources(0, 1, &pSRV);
}

void ForwardPass::SetResources(Direct3D11Renderer& d3dRenderer)
{
}
