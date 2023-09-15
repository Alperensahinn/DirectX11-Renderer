#include "Model_A.h"

MeshData::MeshData(Direct3D11Renderer & pd3dRenderer, std::vector<Vertex>&vertices, std::vector<unsigned int>&indices)
{
	pVertexBuffer = std::make_unique<Direct3D11VertexBuffer<Vertex>>(pd3dRenderer, vertices);
	pIndexBuffer = std::make_unique<Direct3D11IndexBuffer>(pd3dRenderer, indices);
}

void MeshData::Bind(Direct3D11Renderer& pd3dRenderer)
{
	pVertexBuffer.get()->Bind(pd3dRenderer);
	pIndexBuffer.get()->Bind(pd3dRenderer);
}

unsigned int MeshData::GetIndexCount()
{
	return pIndexBuffer.get()->count;
}