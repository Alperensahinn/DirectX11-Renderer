#pragma once

#include "Direct3D11\Direct3D11VertexBuffer.h"
#include "Direct3D11\Direct3D11IndexBuffer.h"
#include <memory>

typedef struct Vertex
{
	float pos_x;
	float pos_y;
	float pos_z;

	float texCoord_u;
	float texCoord_v;
};

class Mesh
{
public:
	Mesh() = default;
	Mesh(Direct3D11Renderer& pd3dRenderer, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void Bind(Direct3D11Renderer& pd3dRenderer);
protected:
	std::unique_ptr<Direct3D11VertexBuffer<Vertex>> pVertexBuffer;
	std::unique_ptr<Direct3D11IndexBuffer> pIndexBuffer;
};