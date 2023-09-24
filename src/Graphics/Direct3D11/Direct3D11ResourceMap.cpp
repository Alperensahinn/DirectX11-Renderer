#include "Direct3D11ResourceMap.h"

std::shared_ptr<Direct3D11VertexShader> Direct3D11ResourceMap::GetD3D11VertexShader(Direct3D11Renderer& d3dRenderer, std::string path)
{
    if (d3d11VertexShaderMap.empty() == true)
    {
        std::shared_ptr<Direct3D11VertexShader> vs = std::make_shared<Direct3D11VertexShader>(d3dRenderer, path);

        d3d11VertexShaderMap.insert(std::make_pair(path, vs));

        return d3d11VertexShaderMap.at(path);
    }

    if(d3d11VertexShaderMap.contains(path))
    {
        return d3d11VertexShaderMap.at(path);
    }

    else
    {
        std::shared_ptr<Direct3D11VertexShader> vs = std::make_shared<Direct3D11VertexShader>(d3dRenderer, path);

        d3d11VertexShaderMap.insert(std::make_pair(path, vs));

        return d3d11VertexShaderMap.at(path);
    }
}

std::shared_ptr<Direct3D11PixelShader> Direct3D11ResourceMap::GetD3D11PixelShader(Direct3D11Renderer& d3dRenderer, std::string path)
{
    if (d3d11PixelShaderMap.empty() == true)
    {
        std::shared_ptr<Direct3D11PixelShader> ps = std::make_shared<Direct3D11PixelShader>(d3dRenderer, path);

        d3d11PixelShaderMap.insert(std::make_pair(path, ps));

        return d3d11PixelShaderMap.at(path);
    }

    if (d3d11PixelShaderMap.contains(path))
    {
        return d3d11PixelShaderMap.at(path);
    }

    else
    {
        std::shared_ptr<Direct3D11PixelShader> ps = std::make_shared<Direct3D11PixelShader>(d3dRenderer, path);

        d3d11PixelShaderMap.insert(std::make_pair(path, ps));

        return d3d11PixelShaderMap.at(path);
    }
}
