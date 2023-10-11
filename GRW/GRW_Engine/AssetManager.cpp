#include "AssetManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"


AssetManager::AssetManager()
{

}

AssetManager* AssetManager::GetAssetManger()
{
	if (Instance == nullptr)
	{
		Instance = new AssetManager();
	}
	return Instance;
}


bool AssetManager::LoadAllAssets()
{
	return false;
}

