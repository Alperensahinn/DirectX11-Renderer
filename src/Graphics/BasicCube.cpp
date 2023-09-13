#include "BasicCube.h"

BasicCube::BasicCube(Direct3D11Renderer& pd3dRenderer) : Mesh()
{
	std::vector<Vertex> vertices;
    vertices.push_back({ 1.0f, 1.0f, -1.0f,						0.0f, 0.0f });
    vertices.push_back({ -1.0f, 1.0f, -1.0f,					1.0f, 0.0f });
    vertices.push_back({ -1.000000f, 1.000000f, 1.000000f,		1.0f, 1.0f });
    vertices.push_back({ 1.000000, 1.000000, 1.000000,			1.0f, 1.0f });
    vertices.push_back({ 1.000000, -1.000000, -1.000000,		0.0f, 1.0f });
    vertices.push_back({ -1.000000f, -1.000000f, -1.000000f,	0.0f, 0.0f });
	vertices.push_back({ -1.000000, -1.000000, 1.000000,		0.0f, 1.0f });
	vertices.push_back({ 1.000000, -1.000000, 1.000000,			0.0f, 0.0f });

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(1);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(7);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(0);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(5);

	pVertexBuffer = std::make_unique<Direct3D11VertexBuffer<Vertex>>(pd3dRenderer, vertices);
	pIndexBuffer = std::make_unique<Direct3D11IndexBuffer>(pd3dRenderer, indices);
}
