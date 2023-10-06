#include "Mesh.h"
#include "GLTFMeshLoader.h"
#include <string>
#include "Vector.h"
#include "Debug.h"
#include "Renderer.h"
#include <comdef.h>

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
	renderer->gfxContext->IASetVertexBuffers(0u, 1u, Submeshes[submesh].VertexBuffer.GetAddressOf(), &stride, &offset);
	renderer->gfxContext->IASetIndexBuffer(Submeshes[submesh].IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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
	return Submeshes[submesh].indices.size();
}

void Mesh::LoadMeshFromFile()
{
	GLTFMeshLoader meshLoader(FilePath.c_str());
	for (int i = 0; i < meshLoader.GetSubmesh(); i++)
	{
		SubMesh newSubMesh;
		meshLoader.GetVertexPositions(i, newSubMesh.positions);
		meshLoader.GetIndices(i, newSubMesh.indices);
		meshLoader.GetNormals(i, newSubMesh.normals);
		meshLoader.GetUVs(i, newSubMesh.uvs);
		meshLoader.GetTangents(i, newSubMesh.tangents);
		Submeshes.push_back(newSubMesh);
	}

}

bool Mesh::CreateBuffers(Renderer* renderer)
{
	HRESULT hr;
	for (int i = 0; i < Submeshes.size(); i++)
	{
		D3D11_BUFFER_DESC verBufferDesc;
		verBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		verBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		verBufferDesc.ByteWidth = sizeof(vertex) * Submeshes[i].VertexList.size();
		verBufferDesc.CPUAccessFlags = 0u;
		verBufferDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA srd;
		srd.pSysMem = Submeshes[i].VertexList.data();

		hr = renderer->gfxDevice->CreateBuffer(&verBufferDesc, &srd, &Submeshes[i].VertexBuffer);
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

		srd.pSysMem = Submeshes[i].indices.data();

		hr = renderer->gfxDevice->CreateBuffer(&indBufferDesc, &srd, &Submeshes[i].IndexBuffer);
		if (FAILED(hr))
		{
			_com_error error(hr);
			LPCTSTR errorText = error.ErrorMessage();
			DEBUG("Failed creating index buffer" << errorText);

			return false;
		}

	}

	return true;
}
