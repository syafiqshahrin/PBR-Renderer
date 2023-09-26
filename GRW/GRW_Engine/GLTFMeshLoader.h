#pragma once
#include "json.hpp"
#include <string>
#include "Vector.h"


struct GLTFMeshInfo
{
	int MaxBufferLength;
	int submeshes;
	std::string MeshName;

	std::vector<int> PosStartOffset;
	std::vector<int> PosByteLength;

	std::vector<int> IndStartOffset;
	std::vector<int> IndByteLength;

};

class GLTFMeshLoader
{
public:
	GLTFMeshLoader(const char *FileAddress);
	~GLTFMeshLoader();

	void GetVertexPositions(std::vector<Vector3> & vPositions);
	void GetIndices(int submeshIndex, std::vector<unsigned int>& indices);
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
	//int MeshBufferMaxByteLength;

	GLTFMeshInfo MeshInfo;
};