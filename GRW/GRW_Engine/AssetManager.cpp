#include "AssetManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "Renderer.h"

AssetManager* AssetManager::Instance = nullptr;

AssetManager::AssetManager()
{

}

void AssetManager::LoadAssetPaths()
{

	//Load texture paths
	std::string path = "../Assets/Textures";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		if (name.find(".Timp") != std::string::npos)
			continue;
		TexturePaths.insert({ name, p });
		DEBUG(name.c_str() << " : " << p.c_str());
	}

	//Load mesh paths
	path = "../Assets/Mesh";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		MeshPaths.insert({ name, p });
		DEBUG(name.c_str() << " : " << p.c_str());
	}

	//Load vertex shader paths
	path = "../Shaders/VertexCSO";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		VertexShaderPaths.insert({ name, p });
		DEBUG(name.c_str() << " : " << p.c_str());
	}


	//Load pixel shader paths
	path = "../Shaders/PixelCSO";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		PixelShaderPaths.insert({ name, p });
		DEBUG(name.c_str() << " : " << p.c_str());
	}


}

void AssetManager::LoadTextureAssets()
{
	std::ifstream f("../Assets/Textures/TextureImportSettings.Timp");
	nlohmann::json TexImporterData = nlohmann::json::parse(f);

	nlohmann::json TexImpSettings = TexImporterData["Textures"];

	for (int i = 0; i < TexImpSettings.size(); i++)
	{
		TextureImportSetting setting;
		setting.name = TexImpSettings[i]["name"];
		setting.Components = TexImpSettings[i]["Components"];
		setting.HDR = TexImpSettings[i]["HDR"];
		setting.BitsPerPixel = TexImpSettings[i]["BitsPerPixel"];
		setting.GenerateMips = TexImpSettings[i]["GenerateMips"];
		setting.Format = static_cast<DXGI_FORMAT>(TexImpSettings[i]["Format"]);

		TextureImportSettings.insert({ setting.name, setting });
	}

	//for each path in texture path map
	for (std::map<std::string, std::string>::iterator it = TexturePaths.begin(); it != TexturePaths.end(); it++)
	{
		TextureImportSetting T = TextureImportSettings[it->first];

		TextureMap.insert({it->first, Texture2D(it->second, T.Components, T.HDR)});
		TextureMap[it->first].CreateTextureFromFile(renderer, T.BitsPerPixel, T.GenerateMips, T.Format);
	}

}

void AssetManager::LoadMeshAssets()
{
	for (std::map<std::string, std::string>::iterator it = MeshPaths.begin(); it != MeshPaths.end(); it++)
	{
		MeshMap.insert({ it->first, Mesh(it->second) });
		DEBUG(it->first.c_str());
		MeshMap[it->first].CreateMeshFromFile(renderer);
	}
}

std::string AssetManager::GetFileNameFromPath(std::string path)
{

	std::istringstream p(path);
	std::vector<std::string> separatedPath;
	std::string s;
	while (std::getline(p, s, '/'))
	{
		separatedPath.push_back(s);
	}


	return separatedPath.back();
}

AssetManager* AssetManager::GetAssetManager()
{
	if (Instance == nullptr)
	{
		Instance = new AssetManager();
	}
	return Instance;
}

void AssetManager::SetRenderer(Renderer* rdr)
{
	GetAssetManager()->renderer = rdr;
}


bool AssetManager::LoadAllAssets()
{
	LoadAssetPaths();
	LoadTextureAssets();
	LoadMeshAssets();
	return false;
}

