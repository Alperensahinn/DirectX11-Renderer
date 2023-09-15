#pragma once

#include "Direct3D11\Direct3D11Drawable.h"
#include "Direct3D11\Direct3D11VertexBuffer.h"
#include "Direct3D11\Direct3D11IndexBuffer.h"

#include "MaterialFactory.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT2 texcoord;
};


class MeshData
{
public:
	MeshData() = default;
	MeshData(Direct3D11Renderer& pd3dRenderer, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void Bind(Direct3D11Renderer& pd3dRenderer);
	unsigned int GetIndexCount();
protected:
	std::unique_ptr<Direct3D11VertexBuffer<Vertex>> pVertexBuffer;
	std::unique_ptr<Direct3D11IndexBuffer> pIndexBuffer;
};


class Mesh : public Direct3D11Drawable
{
public:
	Mesh(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<MeshData>& mesh, std::shared_ptr<Material>& material);
	
	void Draw(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX cameraMatrix, DirectX::FXMMATRIX accumulatedTransform);
	DirectX::XMMATRIX GetModelMatrix() override;
private:
	DirectX::XMFLOAT4X4 transform;
};


class Node
{
	friend class Model;

public: 
	Node(std::vector<Mesh*> meshPtrs, DirectX::XMMATRIX transform);

	void Draw(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX cameraMatrix, DirectX::FXMMATRIX accumulatedTransform);
private:
	void AddChild(std::unique_ptr<Node> pChild);
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};


class Model
{
	friend class Node;

public:
	Model(Direct3D11Renderer& pd3dRenderer, std::string fileName, std::string folderPath);

	std::shared_ptr<Mesh> ParseMesh(Direct3D11Renderer& pd3dRenderer, aiMesh& mesh, const aiMaterial* const* pMaterials, std::string folderPath, int index);

	std::unique_ptr<Node> ParseNode(const aiNode& node);

	void Draw(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX cameraMatrix, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;
};