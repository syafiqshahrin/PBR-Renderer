#pragma once
#include "json.hpp"
#include <string>
#include "Vector.h"


struct GLTFMeshInfo
{
	unsigned int MaxBufferLength;
	unsigned int submeshes;
	std::string MeshName;

	std::vector<int> PosStartOffset;
	std::vector<int> PosByteLength;

	std::vector<int> IndStartOffset;
	std::vector<int> IndByteLength;

	std::vector<int> NormalStartOffset;
	std::vector<int> NormalByteLength;

	std::vector<int> UVStartOffset;
	std::vector<int> UVByteLength;

	std::vector<int> TangentStartOffset;
	std::vector<int> TangentByteLength;
	bool UsingIntegerIndex;
};

class GLTFMeshLoader
{
public:
	GLTFMeshLoader(const char *FileAddress);
	~GLTFMeshLoader();

	void GetVertexPositions(int submeshIndex, std::vector<Vector3> & vPositions);
	void GetIndices(int submeshIndex, std::vector<unsigned int>& indices);
	void GetNormals(int submeshIndex, std::vector<Vector3>& normals);
	void GetUVs(int submeshIndex, std::vector<Vector2>& uvs);
	void GetTangents(int submeshIndex, std::vector<Vector4>& tangent);
	int GetSubmesh();
private:
	void LoadJSonFile();
	void DecodeMeshData();
	std::string FilePath;
	std::string strMap = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
	std::map<char, int> base64Map;
	nlohmann::json GLTFJsonData;
	std::string MeshEncodedData;
	std::vector<std::byte> MeshDecodedData;
	const int StartIndex = 37;

	GLTFMeshInfo MeshInfo;
};