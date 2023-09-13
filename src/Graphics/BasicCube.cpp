#include "BasicCube.h"

BasicCube::BasicCube(Direct3D11Renderer& pd3dRenderer) : Mesh()
{
	std::vector<Vertex> vertices;
    vertices.push_back({ 1.0f, 1.0f, -1.0f,		1.0f, 1.0f }); //0
    vertices.push_back({ -1.0f, 1.0f, -1.0f,	0.0f, 1.0f }); //1
    vertices.push_back({ -1.0f, 1.0f, 1.0f,		1.0f, 1.0f }); //2
    vertices.push_back({ 1.0f, 1.0f, 1.0f,		0.0f, 1.0f }); //3

    vertices.push_back({ 1.0f, -1.0f, -1.0f,	1.0f, 0.0f }); //4
    vertices.push_back({ -1.0f, -1.0f, -1.0f,	0.0f, 0.0f }); //5
	vertices.push_back({ -1.0f, -1.0f, 1.0f,	1.0f, 0.0f }); //6
	vertices.push_back({ 1.0f, -1.0f, 1.0f,		0.0f, 0.0f }); //7

	vertices.push_back({ 1.0f, 1.0f, -1.0f,		1.0f, 0.0f }); //8
	vertices.push_back({ -1.0f, 1.0f, -1.0f,	0.0f, 0.0f }); //9
	vertices.push_back({ -1.0f, 1.0f, 1.0f,		0.0f, 1.0f }); //10
	vertices.push_back({ 1.0f, 1.0f, 1.0f,		1.0f, 1.0f }); //11

	vertices.push_back({ 1.0f, -1.0f, -1.0f,	1.0f, 1.0f }); //12
	vertices.push_back({ -1.0f, -1.0f, -1.0f,	0.0f, 1.0f }); //13
	vertices.push_back({ -1.0f, -1.0f, 1.0f,	0.0f, 0.0f }); //14
	vertices.push_back({ 1.0f, -1.0f, 1.0f,		1.0f, 0.0f }); //15

	vertices.push_back({ 1.0f, 1.0f, -1.0f,		0.0f, 1.0f }); //16
	vertices.push_back({ 1.0f, 1.0f, 1.0f,		1.0f, 1.0f }); //17
	vertices.push_back({ 1.0f, -1.0f, -1.0f,	0.0f, 0.0f }); //18
	vertices.push_back({ 1.0f, -1.0f, 1.0f,		1.0f, 0.0f }); //19

	vertices.push_back({ -1.0f, 1.0f, -1.0f,	1.0f, 1.0f }); //20
	vertices.push_back({ -1.0f, 1.0f, 1.0f,		0.0f, 1.0f }); //21
	vertices.push_back({ -1.0f, -1.0f, -1.0f,	1.0f, 0.0f }); //22
	vertices.push_back({ -1.0f, -1.0f, 1.0f,	0.0f, 0.0f }); //23

	std::vector<unsigned int> indices;
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(11);
	indices.push_back(9);
	indices.push_back(11);
	indices.push_back(8);

	indices.push_back(15);
	indices.push_back(14);
	indices.push_back(13);
	indices.push_back(15);
	indices.push_back(13);
	indices.push_back(12);

	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(4);

	indices.push_back(6);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(3);

	indices.push_back(18);
	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(17);
	indices.push_back(19);

	indices.push_back(22);
	indices.push_back(23);
	indices.push_back(21);
	indices.push_back(22);
	indices.push_back(21);
	indices.push_back(20);

	pVertexBuffer = std::make_unique<Direct3D11VertexBuffer<Vertex>>(pd3dRenderer, vertices);
	pIndexBuffer = std::make_unique<Direct3D11IndexBuffer>(pd3dRenderer, indices);
}
