#pragma once
#include <string>
#include <vector>
#include <map>
#include <type_traits>


class Texture2D;
class TextureCube;
class Mesh;
class VertexShader;
class PixelShader;
class Renderer;
struct TextureImportSetting;


class AssetManager
{
public:
	static AssetManager* GetAssetManager();
	static void SetRenderer(Renderer* rdr);
	AssetManager(const AssetManager& obj) = delete;
	~AssetManager();

	bool LoadAllAssets();
	template<typename T>
	T& GetAsset(std::string const & name);
	


private:
	AssetManager();
	static AssetManager* Instance;
	Renderer* renderer;
	std::map<std::string, TextureImportSetting> TextureImportSettings;

	std::map<std::string,std::string> TexturePaths;
	std::map<std::string,std::string> MeshPaths;
	std::map<std::string,std::string> VertexShaderPaths;
	std::map<std::string,std::string> PixelShaderPaths;

	std::map<std::string, Texture2D> TextureMap;
	std::map<std::string, Mesh> MeshMap;
	std::map<std::string, VertexShader> VertexShaderMap;
	std::map<std::string, PixelShader> PixelShaderMap;
	

	void LoadAssetPaths();
	void LoadTextureAssets();
	void LoadMeshAssets();
	void LoadShaderAssets();

	std::string GetFileNameFromPath(std::string path);

};



template<>
inline Texture2D& AssetManager::GetAsset<Texture2D>(std::string const& name)
{
	return TextureMap[name];

}

template<>
inline Mesh& AssetManager::GetAsset<Mesh>(std::string const& name)
{
	return MeshMap[name];
}

