#include "Model.h"
#include "Direct3D11\Direct3D11ResourceMap.h"

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

Mesh::Mesh(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<MeshData>& mesh, std::shared_ptr<Material>& material) : Direct3D11Drawable(pd3dRenderer, mesh, material)
{
}

void Mesh::Draw(Direct3D11Renderer& pd3dRenderer, DirectX::FXMMATRIX accumulatedTransform)
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Direct3D11Drawable::Draw(pd3dRenderer);
}

DirectX::XMMATRIX Mesh::GetModelMatrix()
{
	return DirectX::XMLoadFloat4x4(&transform);
}

Node::Node(std::vector<Mesh*> meshPtrs, DirectX::XMMATRIX transform) : meshPtrs(std::move(meshPtrs))
{
	DirectX::XMStoreFloat4x4(&this->transform, transform);
}

void Node::Draw(Direct3D11Renderer& pd3dRenderer, DirectX::FXMMATRIX accumulatedTransform)
{
	auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;

	for (auto pm : meshPtrs)
	{
		pm->Draw(pd3dRenderer, built);
	}

	for (auto& pc : childPtrs)
	{
		pc->Draw(pd3dRenderer, built);
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild)
{
	childPtrs.push_back(std::move(pChild));
}

Model::Model(Direct3D11Renderer& d3dRenderer, std::string fileName, std::string folderPath)
{
	Assimp::Importer importer;
	auto pScene = importer.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace
	);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(d3dRenderer, *pScene->mMeshes[i], pScene->mMaterials, folderPath, i));
	}

	pRoot = ParseNode(*pScene->mRootNode);
}

std::shared_ptr<Mesh> Model::ParseMesh(Direct3D11Renderer& d3dRenderer, aiMesh& mesh, const aiMaterial* const* pMaterials, std::string folderPath, int index)
{
	std::shared_ptr<Direct3D11Texture2D> albedoTexture;
	std::shared_ptr<Direct3D11SamplerState> samplerState;
	std::shared_ptr<Direct3D11VertexBuffer<Vertex>> vertexBuffer;
	std::shared_ptr<Direct3D11IndexBuffer> indexBuffer;
	std::shared_ptr<Direct3D11VertexShader> vertexShader;
	std::shared_ptr<Direct3D11PixelShader> pixelShader;
	std::shared_ptr<Direct3D11InputLayout> inputLayout;

	//textures
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texFileName;

		const char* fileNamePtr;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			fileNamePtr = texFileName.C_Str();
			std::string filePath = fileNamePtr;
			filePath = folderPath + filePath;


			albedoTexture = std::make_shared<Direct3D11Texture2D>(d3dRenderer, filePath, 1u);
		}
		else
		{
			albedoTexture = std::make_shared<Direct3D11Texture2D>(d3dRenderer, "Resources\\Textures\\Default_Albedo.png", 1u);
		}

		/*
		if (material.GetTexture(aiTextureType_HEIGHT, 0, &texFileName) == aiReturn_SUCCESS)
		{
			fileNamePtr = texFileName.C_Str();
			std::string filePath = fileNamePtr;
			filePath = folderPath + filePath;

			bindablePtrs.push_back(Texture::Resolve(gfx, filePath, 1u));
		}
		else
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, "resources\\Defaults\\Textures\\Default_Normal.png", 1u));
		}
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			fileNamePtr = texFileName.C_Str();
			std::string filePath = fileNamePtr;
			filePath = folderPath + filePath;

			bindablePtrs.push_back(Texture::Resolve(gfx, filePath, 2u));
		}
		else
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, "resources\\Defaults\\Textures\\Default_Specular.png", 2u));
		}
		*/

		samplerState = std::make_shared<Direct3D11SamplerState>(d3dRenderer);
	}

	else
	{
		albedoTexture = std::make_shared<Direct3D11Texture2D>(d3dRenderer, "Resources\\Textures\\Default_Albedo.png", 1u);
		//bindablePtrs.push_back(Texture::Resolve(gfx, "resources\\Defaults\\Textures\\Default_Diffuse.png", 0u));
		//bindablePtrs.push_back(Texture::Resolve(gfx, "resources\\Defaults\\Textures\\Default_Normal.png", 1u));
		//bindablePtrs.push_back(Texture::Resolve(gfx, "resources\\Defaults\\Textures\\Default_Specular.png", 2u));
		samplerState = std::make_shared<Direct3D11SamplerState>(d3dRenderer);
	}

	std::vector<Vertex> vertices;
	vertices.reserve(mesh.mNumVertices);

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vertices.push_back({
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mBitangents[i]),
			*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
			});
	}

	std::vector<unsigned int> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		indices.push_back(mesh.mFaces[i].mIndices[0]);
		indices.push_back(mesh.mFaces[i].mIndices[1]);
		indices.push_back(mesh.mFaces[i].mIndices[2]);
	}

	std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>(d3dRenderer, vertices, indices);

	//shaders
	vertexShader = d3dRenderer.GetResourceMap().GetD3D11VertexShader(d3dRenderer, "x64\\Debug\\VertexShaderTest.cso");
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({ "Position", 0u,  DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "Normal", 0u,  DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "Tangent", 0u,  DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "Bitangent", 0u,  DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "TexCoord", 0u,  DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	inputLayout = std::make_shared<Direct3D11InputLayout>(d3dRenderer, ied, vertexShader.get()->GetBlob());

	pixelShader = d3dRenderer.GetResourceMap().GetD3D11PixelShader(d3dRenderer, "x64\\Debug\\PixelShaderTest.cso");


	Material::MATERIAL_DESC materialDesc = {};
	materialDesc.VertexShader = vertexShader;
	materialDesc.PixelShader = pixelShader;
	materialDesc.InputLayout = inputLayout;
	materialDesc.SamplerState = samplerState;
	materialDesc.AlbedoTexture = albedoTexture;

	std::shared_ptr<Material> material = std::make_shared<Material>(materialDesc);

	return std::make_shared<Mesh>(d3dRenderer, meshData, material);
}

std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	auto transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);

	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode.get()->AddChild(ParseNode(*node.mChildren[i]));
	}

	return pNode;
}

void Model::Draw(Direct3D11Renderer& pd3dRenderer, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	pRoot->Draw(pd3dRenderer, DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0)
		* DirectX::XMMatrixScaling(scaleX, scaleY, scaleZ)
		* DirectX::XMMatrixTranslation(posX, posY, posZ));
}
