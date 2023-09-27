#include "GLTFMeshLoader.h"
#include "json.hpp"
#include <fstream>
#include <bitset>
#include "Debug.h"



GLTFMeshLoader::GLTFMeshLoader(const char *FileAddress)
{
	FilePath = FileAddress;

	for (int i = 0; i < 64; i++)
	{
		base64Map.insert({ strMap[i], i });
	}
	DEBUG("Loading File");
	LoadJSonFile();
	DEBUG("Decoding Mesh Data");
	DecodeMeshData();
}

GLTFMeshLoader::~GLTFMeshLoader()
{
	DEBUG("Destructor Called");
}

void GLTFMeshLoader::GetVertexPositions(std::vector<Vector3> & vPositions)
{
	std::vector<float> f;
	
	for (int i = 0; i < MeshInfo.submeshes; i++)
	{
		int start = MeshInfo.PosStartOffset[i];
		int maxByteLength = start + MeshInfo.PosByteLength[i];


		for (unsigned int j = start, k = 0; j < maxByteLength; j += 4, k++)
		{
			std::byte fl[4];
			fl[0] = MeshDecodedData[j];
			fl[1] = MeshDecodedData[j+1];
			fl[2] = MeshDecodedData[j+2];
			fl[3] = MeshDecodedData[j+3];

			f.push_back(0);
			std::memcpy(&f[k], fl, sizeof(float));
			//DEBUG("F: " << f[k]);
		}

	}

	for (int i = 0, k = 0; i < f.size(); i+=3, k++)
	{
		vPositions.push_back(Vector3(f[i], f[i + 1], f[i + 2]));
	}

}



void GLTFMeshLoader::GetIndices(int submeshIndex, std::vector<unsigned int>& indices)
{
	int start = MeshInfo.IndStartOffset[submeshIndex];
	int maxByteLength = start + MeshInfo.IndByteLength[submeshIndex];

	std::vector<unsigned int> s;

	for (unsigned int j = start, k = 0; j < maxByteLength; j += 2, k++)
	{
		std::byte sl[2];
		char cl[4];

		sl[0] = MeshDecodedData[j];
		sl[1] = MeshDecodedData[j+1];

	

		//std::string test = cl;

		s.push_back(0);
		std::memcpy(&s[k], sl, sizeof(unsigned short));
		//s[k] = test1;
	}


	for (int i = 0, k = 0; i < s.size(); i++, k++)
	{
		//unsigned int t = 0;
		//t = t | s[i];
		indices.push_back((unsigned int)s[i]);
	}
	DEBUG("indices: " << indices.size());
}

void GLTFMeshLoader::GetNormals(int submeshIndex, std::vector<Vector3>& normals)
{
	int start = MeshInfo.NormalStartOffset[submeshIndex];
	int maxByteLength = start + MeshInfo.NormalByteLength[submeshIndex];
	std::vector<float> f;
	for (unsigned int j = start, k = 0; j < maxByteLength; j += 4, k++)
	{
		std::byte fl[4];
		fl[0] = MeshDecodedData[j];
		fl[1] = MeshDecodedData[j + 1];
		fl[2] = MeshDecodedData[j + 2];
		fl[3] = MeshDecodedData[j + 3];

		f.push_back(0);
		std::memcpy(&f[k], fl, sizeof(float));
	}
	
	for (int i = 0, k = 0; i < f.size(); i += 3, k++)
	{
		normals.push_back(Vector3(f[i], f[i + 1], f[i + 2]));
	}

	
}

void GLTFMeshLoader::GetUVs(int submeshIndex, std::vector<Vector2>& uvs)
{
	int start = MeshInfo.UVStartOffset[submeshIndex];
	int maxByteLength = start + MeshInfo.UVByteLength[submeshIndex];
	std::vector<float> f;
	for (unsigned int j = start, k = 0; j < maxByteLength; j += 4, k++)
	{
		std::byte fl[4];
		fl[0] = MeshDecodedData[j];
		fl[1] = MeshDecodedData[j + 1];
		fl[2] = MeshDecodedData[j + 2];
		fl[3] = MeshDecodedData[j + 3];

		f.push_back(0);
		std::memcpy(&f[k], fl, sizeof(float));
	}

	for (int i = 0, k = 0; i < f.size(); i += 2, k++)
	{
		uvs.push_back(Vector2(f[i], f[i + 1]));
	}
	

}

void GLTFMeshLoader::LoadJSonFile()
{
	std::ifstream f(FilePath.c_str());
	GLTFJsonData = nlohmann::json::parse(f);

	MeshEncodedData = GLTFJsonData["buffers"][0]["uri"];
	MeshInfo.MaxBufferLength = GLTFJsonData["buffers"][0]["byteLength"];

	MeshInfo.MeshName = GLTFJsonData["meshes"][0]["name"];
	MeshInfo.submeshes = GLTFJsonData["meshes"][0]["primitives"].size();

	for (int i = 0; i < MeshInfo.submeshes; i++)
	{
		int accessorPosIndex = GLTFJsonData["meshes"][0]["primitives"][i]["attributes"]["POSITION"];
		int bufferViewPosIndex = GLTFJsonData["accessors"][accessorPosIndex]["bufferView"];
		MeshInfo.PosStartOffset.push_back(GLTFJsonData["bufferViews"][bufferViewPosIndex]["byteOffset"]);
		MeshInfo.PosByteLength.push_back(GLTFJsonData["bufferViews"][bufferViewPosIndex]["byteLength"]);

		int accessorIndIndex = GLTFJsonData["meshes"][0]["primitives"][i]["indices"];
		int bufferViewIndIndex = GLTFJsonData["accessors"][accessorIndIndex]["bufferView"];

		MeshInfo.IndStartOffset.push_back(GLTFJsonData["bufferViews"][bufferViewIndIndex]["byteOffset"]);
		MeshInfo.IndByteLength.push_back(GLTFJsonData["bufferViews"][bufferViewIndIndex]["byteLength"]);

		int accessorNormalIndex = GLTFJsonData["meshes"][0]["primitives"][i]["attributes"]["NORMAL"];
		int bufferViewNormalIndex = GLTFJsonData["accessors"][accessorNormalIndex]["bufferView"];
		MeshInfo.NormalStartOffset.push_back(GLTFJsonData["bufferViews"][bufferViewNormalIndex]["byteOffset"]);
		MeshInfo.NormalByteLength.push_back(GLTFJsonData["bufferViews"][bufferViewNormalIndex]["byteLength"]);

		int accessorUVIndex = GLTFJsonData["meshes"][0]["primitives"][i]["attributes"]["TEXCOORD_0"];
		int bufferViewUVIndex = GLTFJsonData["accessors"][accessorUVIndex]["bufferView"];
		MeshInfo.UVStartOffset.push_back(GLTFJsonData["bufferViews"][bufferViewUVIndex]["byteOffset"]);
		MeshInfo.UVByteLength.push_back(GLTFJsonData["bufferViews"][bufferViewUVIndex]["byteLength"]);

	}
}

void GLTFMeshLoader::DecodeMeshData()
{
	int maxLength = MeshEncodedData.size();
	for (int i = StartIndex; i < maxLength; i+=4)
	{
		//DEBUG("Decoding Mesh Data at: " << i);
		int TempDataStore = 0;
		char char3[3];
		int t = 0;
		bool end = false;
		//DEBUG(i);
		for(int j = 0; j < 4; j++)
		{

			//DEBUG(MeshEncodedData[i + j]);
			if (MeshEncodedData[i + j] == '=')
			{
				end = true;
				t++;
				//DEBUG("END");
			}
			int temp = base64Map[MeshEncodedData[i + j]];
			TempDataStore = TempDataStore | temp;
			if(j < 3)
				TempDataStore = TempDataStore << 6;
			
		}

		char3[2] = TempDataStore & 0xFF;
		TempDataStore = TempDataStore >> 8;

		char3[1] = TempDataStore & 0xFF;
		TempDataStore = TempDataStore >> 8;

		char3[0] = TempDataStore & 0xFF;
		
		if (end && t >= 1)
		{
			MeshDecodedData.push_back((std::byte)char3[0]);
			if(t == 1)
				MeshDecodedData.push_back((std::byte)char3[1]);

			continue;
		}

		for (int k = 0; k < 3; k++)
		{
			MeshDecodedData.push_back((std::byte)char3[k]);
		}
	}

	DEBUG("DecodedData Byte Length: " << MeshDecodedData.size());
	DEBUG("Correct Byte Length: " << MeshInfo.MaxBufferLength);

}


