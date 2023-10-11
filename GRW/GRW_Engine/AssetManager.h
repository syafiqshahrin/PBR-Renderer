#pragma once
#include <string>
#include <vector>
#include <map>

class Texture2D;
class TextureCube;
class Mesh;
class VertexShader;
class PixelShader;



class AssetManager
{
public:
	static AssetManager* GetAssetManger();
	AssetManager(const AssetManager& obj) = delete;
	~AssetManager();

	bool LoadAllAssets();
	template<typename T>
	bool GetAsset(std::string const & name, T &asset);


private:
	AssetManager();
	static AssetManager* Instance;



	std::vector<std::string> TexturePaths;
	std::vector<std::string> MeshPaths;
	std::vector<std::string> VertexShaderPaths;
	std::vector<std::string> PixelShaderPaths;

	std::map<std::string, Texture2D> TextureMap;
	std::map<std::string, Mesh> MeshMap;
	std::map<std::string, VertexShader> VertexShaderMap;
	std::map<std::string, PixelShader> PixelShaderMap;
	

	void LoadAssetPaths();
	void LoadTextureAssets();
	void LoadMeshAssets();
	void LoadShaderAssets();

	

};

AssetManager* AssetManager::GetAssetManger()
{
	if (Instance == nullptr)
	{
		Instance = new AssetManager();
	}
	return Instance;
}