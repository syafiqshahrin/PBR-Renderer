#pragma once
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include "json.hpp"
#include <fstream>
#include "Material.h"
#include"Texture.h"

class Mesh;
class VertexShader;
class PixelShader;
class Renderer;
struct TextureImportSetting;
//class MaterialAsset;
//struct MaterialAssetData;
struct MaterialShader;

class AssetManager
{
public:
	static AssetManager* GetAssetManager();
	static void SetRenderer(Renderer* rdr);
	AssetManager(const AssetManager& obj) = delete;
	~AssetManager();

	bool LoadAllAssets();
	template<typename T>
	T* GetAsset(std::string const & name);
	
	void GetAllLoadedTextureNames(std::vector<std::string>& names);
	void GetAllLoadedMeshNames(std::vector<std::string> &names);
	void GetAllLoadedMaterialNames(std::vector<std::string>& names);
	void GetAllLoadedMaterialShaderNames(std::vector<std::string>& names);
	
	void CreateMaterialDataAsset(std::string name, std::string matShader);

	template<typename T>
	void SaveAsset(std::string const& name);

private:
	AssetManager();
	static AssetManager* Instance;
	Renderer* renderer;
	std::map<std::string, TextureImportSetting> TextureImportSettings;
	std::map<std::string, MaterialAssetData> MaterialImportData;
	std::map<std::string, MaterialShader> MaterialShaderImportData;

	std::map<std::string,std::string> TexturePaths;
	std::map<std::string,std::string> MeshPaths;
	std::map<std::string,std::string> VertexShaderPaths;
	std::map<std::string,std::string> PixelShaderPaths;
	std::map<std::string,std::string> MaterialAssetPaths;
	std::map<std::string,std::string> MaterialShaderPaths;

	std::map<std::string, Texture2D> TextureMap;
	std::map<std::string, Mesh> MeshMap;
	std::map<std::string, VertexShader> VertexShaderMap;
	std::map<std::string, PixelShader> PixelShaderMap;
	std::map<std::string, MaterialAsset> MaterialAssetMap;

	void LoadAssetPaths();
	void LoadTextureAssets();
	void LoadMeshAssets();
	void LoadShaderAssets();
	void LoadMaterialAssets();

	std::string GetFileNameFromPath(std::string path);

};


template<>
inline MaterialAsset* AssetManager::GetAsset<MaterialAsset>(std::string const& name)
{
	return &MaterialAssetMap[name];
}

template<>
inline MaterialAssetData* AssetManager::GetAsset<MaterialAssetData>(std::string const& name)
{
	return &MaterialImportData[name];
}

template<>
inline Texture2D* AssetManager::GetAsset<Texture2D>(std::string const& name)
{
	return &TextureMap[name];

}

template<>
inline Mesh* AssetManager::GetAsset<Mesh>(std::string const& name)
{
	return &MeshMap[name];
}

template<>
inline VertexShader* AssetManager::GetAsset<VertexShader>(std::string const& name)
{
	return &VertexShaderMap[name];
}

template<>
inline PixelShader* AssetManager::GetAsset<PixelShader>(std::string const& name)
{
	return &PixelShaderMap[name];
}

template<>
inline MaterialShader* AssetManager::GetAsset<MaterialShader>(std::string const& name)
{
	return &MaterialShaderImportData[name];
}

template<>
inline void AssetManager::SaveAsset<MaterialAssetData>(std::string const& name)
{
	MaterialAssetData* m = &MaterialImportData[name];
	std::ifstream f(MaterialAssetPaths[name]);

	nlohmann::json mimp = nlohmann::json::parse(f);

	mimp["Material"]["Name"] = m->Name;

	for (int i = 0; i < m->textureParams.size(); i++)
	{
		mimp["Material"]["Textures"][i]["ParameterName"] = m->textureParams[i].paramName;
		mimp["Material"]["Textures"][i]["TextureName"] = m->textureParams[i].texture->GetName();
		mimp["Material"]["Textures"][i]["BindSlot"] = m->textureParams[i].bindSlot;
	}

	for (int i = 0; i < m->parameters.size(); i++)
	{
		mimp["Material"]["Parameters"][i]["ParamName"] = m->parameters[i].paramName;
		mimp["Material"]["Parameters"][i]["Type"] = static_cast<int>(m->parameters[i].type);
		mimp["Material"]["Parameters"][i]["Offset"] = m->parameters[i].offset;
		mimp["Material"]["Parameters"][i]["ScalarValue"] = m->parameters[i].valueF;
		mimp["Material"]["Parameters"][i]["VectorValue"][0] = m->parameters[i].valueV.x;
		mimp["Material"]["Parameters"][i]["VectorValue"][1] = m->parameters[i].valueV.y;
		mimp["Material"]["Parameters"][i]["VectorValue"][2] = m->parameters[i].valueV.z;
		mimp["Material"]["Parameters"][i]["VectorValue"][3] = m->parameters[i].valueV.w;
	}

	mimp["Material"]["Shader"]["Vertex"] = m->ms->VSName;
	mimp["Material"]["Shader"]["Pixel"] = m->ms->PSName;

	mimp["Material"]["Blend"] = static_cast<int>(m->blend);
	mimp["Material"]["Cull"] = static_cast<int>(m->cull);
	mimp["Material"]["Fill"] = static_cast<int>(m->fill);

	std::ofstream o(MaterialAssetPaths[name]);
	o << std::setw(4) << mimp << std::endl;

}