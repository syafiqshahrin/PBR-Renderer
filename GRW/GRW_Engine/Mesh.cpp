#include "Mesh.h"
#include "GLTFMeshLoader.h"
#include <string>
#include "Vector.h"
#include "Debug.h"
#include "Renderer.h"
#include <comdef.h>

Mesh::Mesh()
{
}

Mesh::Mesh(std::string const &filePath)
{
	FilePath = filePath;
}

Mesh::~Mesh()
{

}

bool Mesh::CreateMeshFromFile(Renderer* renderer)
{
	LoadMeshFromFile();
	UpdateVertexList();
	CreateBuffers(renderer);

	return false;
}

void Mesh::BindMesh(int submesh, Renderer* renderer)
{
	const UINT stride = sizeof(vertex);
	const UINT offset = 0u;
	renderer->gfxContext->IASetVertexBuffers(0u, 1u, MeshBuffers[submesh].VertexBuffer.GetAddressOf(), &stride, &offset);
	renderer->gfxContext->IASetIndexBuffer(MeshBuffers[submesh].IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	renderer->gfxContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::UpdateVertexList()
{
	for (int i = 0; i <Submeshes.size(); i++)
	{
		for (int j = 0; j < Submeshes[i].positions.size(); j++)
		{
			vertex newVert(Submeshes[i].positions[j], Submeshes[i].normals[j], Submeshes[i].uvs[j], Submeshes[i].tangents[j], 0, 1, 0);
			Submeshes[i].VertexList.push_back(newVert);

		}
	}
}

int Mesh::GetIndexListSize(int submesh)
{
	return MeshBuffers[submesh].indicesSize;
}

void Mesh::LoadMeshFromFile()
{
	GLTFMeshLoader meshLoader(FilePath.c_str());
	for (int i = 0; i < meshLoader.GetSubmesh(); i++)
	{
		SubMesh newSubMesh;
		SubMeshBuffer newBuffer;
		meshLoader.GetVertexPositions(i, newSubMesh.positions);
		meshLoader.GetIndices(i, newSubMesh.indices);
		meshLoader.GetNormals(i, newSubMesh.normals);
		meshLoader.GetUVs(i, newSubMesh.uvs);
		meshLoader.GetTangents(i, newSubMesh.tangents);
		newBuffer.indicesSize = newSubMesh.indices.size();
		Submeshes.push_back(newSubMesh);
		MeshBuffers.push_back(newBuffer);
	}

}

bool Mesh::CreateBuffers(Renderer* renderer)
{
	HRESULT hr;
	for (int i = 0; i < MeshBuffers.size(); i++)
	{
		D3D11_BUFFER_DESC verBufferDesc;
		verBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		verBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		verBufferDesc.ByteWidth = sizeof(vertex) * Submeshes[i].VertexList.size();
		verBufferDesc.CPUAccessFlags = 0u;
		verBufferDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA srd;

		std::vector<vertex> pos = Submeshes[i].VertexList;
		//std::copy(Submeshes[i].VertexList.begin(), Submeshes[i].VertexList.end(), pos);

		//srd.pSysMem = Submeshes[i].VertexList.data();
		srd.pSysMem = pos.data();

		hr = renderer->gfxDevice->CreateBuffer(&verBufferDesc, &srd, &MeshBuffers[i].VertexBuffer);
		if (FAILED(hr))
		{
			_com_error error(hr);
			LPCTSTR errorText = error.ErrorMessage();
			DEBUG("Failed creating vertex buffer" << errorText);

			return false;
		}

		const UINT stride = sizeof(vertex);
		const UINT offset = 0u;
		//renderer->gfxContext->IASetVertexBuffers(0u, 1u, Submeshes[i].VertexBuffer.GetAddressOf(), &stride, &offset);


		D3D11_BUFFER_DESC indBufferDesc;
		indBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indBufferDesc.ByteWidth = sizeof(unsigned int) * Submeshes[i].indices.size(); //size of the whole array
		indBufferDesc.CPUAccessFlags = 0u;
		indBufferDesc.MiscFlags = 0u;

		//std::vector<unsigned int> ind;
		//std::copy(Submeshes[i].indices.begin(), Submeshes[i].indices.end(), ind);

		srd.pSysMem = Submeshes[i].indices.data();
		//srd.pSysMem = ind.data();

		hr = renderer->gfxDevice->CreateBuffer(&indBufferDesc, &srd, &MeshBuffers[i].IndexBuffer);
		if (FAILED(hr))
		{
			_com_error error(hr);
			LPCTSTR errorText = error.ErrorMessage();
			DEBUG("Failed creating index buffer" << errorText);

			return false;
		}



		//free up memory
		/*
		Submeshes[i].VertexList.clear();
		Submeshes[i].VertexList.shrink_to_fit();
		Submeshes[i].positions.clear();
		Submeshes[i].positions.shrink_to_fit();
		Submeshes[i].uvs.clear();
		Submeshes[i].uvs.shrink_to_fit();
		Submeshes[i].tangents.clear();
		Submeshes[i].tangents.shrink_to_fit();
		Submeshes[i].normals.clear();
		Submeshes[i].normals.shrink_to_fit();
		*/
	}

	//free up memory
	Submeshes.clear();
	Submeshes.shrink_to_fit();
	

	return true;
}
