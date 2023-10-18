#include "AssetManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "Renderer.h"
#include "Material.h"

AssetManager* AssetManager::Instance = nullptr;


void AssetManager::GetAllLoadedTextureNames(std::vector<std::string> &names)
{
	for (std::map<std::string, std::string>::iterator it = TexturePaths.begin(); it != TexturePaths.end(); it++)
	{
		names.push_back(it->first);
	}

}

void AssetManager::GetAllLoadedMeshNames(std::vector<std::string>& names)
{
	for (std::map<std::string, std::string>::iterator it = MeshPaths.begin(); it != MeshPaths.end(); it++)
	{
		names.push_back(it->first);
	}
}

void AssetManager::GetAllLoadedMaterialNames(std::vector<std::string>& names)
{
	for (std::map<std::string, std::string>::iterator it = MaterialAssetPaths.begin(); it != MaterialAssetPaths.end(); it++)
	{
		names.push_back(it->first);
	}

}

void AssetManager::GetAllLoadedMaterialShaderNames(std::vector<std::string>& names)
{
	for (std::map<std::string, std::string>::iterator it = MaterialShaderPaths.begin(); it != MaterialShaderPaths.end(); it++)
	{
		names.push_back(it->first);
	}

}

void AssetManager::CreateMaterialDataAsset(std::string name, std::string matShader)
{
	MaterialShader* ms = &MaterialShaderImportData[matShader];
	nlohmann::json mimp;
	//mimp["Material"];
	mimp["Material"]["Name"] = name;
	for (int i = 0; i < ms->TextureParameters.size(); i++)
	{
		mimp["Material"]["Textures"].push_back({ {"BindSlot", ms->TextureParameters[i].bindSlot}, {"ParameterName", ms->TextureParameters[i].paramName}, {"TextureName", ms->TextureParameters[i].defaultTexture}});
	}
	for (int i = 0; i < ms->ShaderParameters.size(); i++)
	{
		mimp["Material"]["Parameters"].push_back
			({	{"Offset", ms->ShaderParameters[i].offset}, 
				{"ParamName", ms->ShaderParameters[i].paramName}, 
				{"Type", ms->ShaderParameters[i].paramType}, 
				{"ScalarValue", ms->ShaderParameters[i].defaultScalarValue},
				{"VectorValue", 
					{ms->ShaderParameters[i].defaultVectorValue.x, 
					ms->ShaderParameters[i].defaultVectorValue.y, 
					ms->ShaderParameters[i].defaultVectorValue.z,
					ms->ShaderParameters[i].defaultVectorValue.w} 
				}
			});
	}
	mimp["Material"]["Shader"]["MaterialShader"] = matShader;
	mimp["Material"]["Shader"]["Pixel"] = ms->PSName;
	mimp["Material"]["Shader"]["Vertex"] = ms->VSName;

	mimp["Material"]["Cull"] = 0;
	mimp["Material"]["Blend"] = 0;
	mimp["Material"]["Fill"] = 0;

	std::string fName = name + ".mimp";
	std::string path = "../Assets/Materials/" + fName;
	
	MaterialAssetPaths.insert({ fName, path});

	//Create material asset data
	MaterialAssetData matData;

	int textureParamSize = mimp["Material"]["Textures"].size();
	for (int i = 0; i < textureParamSize; i++)
	{
		TexParam tParam;
		tParam.paramName = mimp["Material"]["Textures"][i]["ParameterName"];
		std::string texName = mimp["Material"]["Textures"][i]["TextureName"];
		tParam.texture = &TextureMap[texName];
		tParam.IsRenderTexture = false;
		tParam.bindSlot = mimp["Material"]["Textures"][i]["BindSlot"];
		matData.textureParams.push_back(tParam);
	}

	int paramSize = mimp["Material"]["Parameters"].size();
	for (int i = 0; i < paramSize; i++)
	{
		ShaderParam param;
		param.paramName = mimp["Material"]["Parameters"][i]["ParamName"];
		param.type = static_cast<ShaderParamType>(mimp["Material"]["Parameters"][i]["Type"]);
		param.offset = mimp["Material"]["Parameters"][i]["Offset"];
		param.valueF = mimp["Material"]["Parameters"][i]["ScalarValue"];
		param.valueV.x = mimp["Material"]["Parameters"][i]["VectorValue"][0];
		param.valueV.y = mimp["Material"]["Parameters"][i]["VectorValue"][1];
		param.valueV.z = mimp["Material"]["Parameters"][i]["VectorValue"][2];
		param.valueV.w = mimp["Material"]["Parameters"][i]["VectorValue"][3];
		matData.parameters.push_back(param);
		matData.parametersMap.insert({ param.paramName, param });
	}

	matData.blend = static_cast<BlendMode>(mimp["Material"]["Blend"]);
	matData.cull = static_cast<CullMode>(mimp["Material"]["Cull"]);
	matData.fill = static_cast<FillMode>(mimp["Material"]["Fill"]);

	std::string vsName = mimp["Material"]["Shader"]["Vertex"];
	std::string psName = mimp["Material"]["Shader"]["Pixel"];
	matData.vs = &VertexShaderMap[vsName];
	matData.ps = &PixelShaderMap[psName];
	matData.ms = &MaterialShaderImportData[mimp["Material"]["Shader"]["MaterialShader"]];
	matData.Name = mimp["Material"]["Name"];
	MaterialImportData.insert({ fName, matData });

	//Create material
	MaterialAssetMap.insert({ fName, MaterialAsset() });
	MaterialAssetMap[fName].CreateMaterial(renderer, &matData);

	//Save asset to disk
	std::ofstream o(path);
	o << std::setw(4) << mimp << std::endl;
}

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

	//Load Material Shader paths
	path = "../Shaders/MaterialShaders";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		if (name.find(".msimp") != std::string::npos)
		{
			MaterialShaderPaths.insert({ name, p });
			DEBUG(name.c_str() << " : " << p.c_str());
		}
	}
	
	//Load material paths
	path = "../Assets/Materials";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string p = entry.path().string();
		p.replace(p.find("\\"), 1, "/");
		std::string name = GetFileNameFromPath(p);
		if (name.find(".mimp") != std::string::npos)
		{
			MaterialAssetPaths.insert({ name, p });
			DEBUG(name.c_str() << " : " << p.c_str());
		}
	
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

		TextureMap.insert({it->first, Texture2D(it->first, it->second, T.Components, T.HDR)});
		TextureMap[it->first].CreateTextureFromFile(renderer, T.BitsPerPixel, T.GenerateMips, T.Format);
	}

}

void AssetManager::LoadMeshAssets()
{
	for (std::map<std::string, std::string>::iterator it = MeshPaths.begin(); it != MeshPaths.end(); it++)
	{
		MeshMap.insert({ it->first, Mesh(it->second) });
		MeshMap[it->first].CreateMeshFromFile(renderer);
	}
}

void AssetManager::LoadShaderAssets()
{
	for (std::map<std::string, std::string>::iterator it = VertexShaderPaths.begin(); it != VertexShaderPaths.end(); it++)
	{
		VertexShaderMap.insert({ it->first, VertexShader(it->second) });
		VertexShaderMap[it->first].CreateShader(renderer);
	}

	for (std::map<std::string, std::string>::iterator it = PixelShaderPaths.begin(); it != PixelShaderPaths.end(); it++)
	{
		PixelShaderMap.insert({ it->first, PixelShader(it->second) });
		PixelShaderMap[it->first].CreateShader(renderer);
	}

	for (std::map<std::string, std::string>::iterator it = MaterialShaderPaths.begin(); it != MaterialShaderPaths.end(); it++)
	{
		std::ifstream f(it->second);
		nlohmann::json  materialShaderImport = nlohmann::json::parse(f);
		nlohmann::json materialShaderData = materialShaderImport["MaterialShader"];
		MaterialShader materialShader;

		materialShader.MaterialShaderName = materialShaderData["Name"];
		materialShader.VSName = materialShaderData["Shader"]["Vertex"];
		materialShader.PSName = materialShaderData["Shader"]["Pixel"];

		int textureParamSize = materialShaderData["Textures"].size();

		for (int i = 0; i < textureParamSize; i++)
		{
			ShaderTextureParam tParam;
			tParam.paramName = materialShaderData["Textures"][i]["ParameterName"];
			tParam.defaultTexture = materialShaderData["Textures"][i]["DefaultTextureName"];
			tParam.bindSlot = materialShaderData["Textures"][i]["BindSlot"];
			materialShader.TextureParameters.push_back(tParam);
		}

		int paramSize = materialShaderData["Parameters"].size();
		for (int i = 0; i < paramSize; i++)
		{
			ShaderConstantParam param;
			param.paramName = materialShaderData["Parameters"][i]["ParamName"];
			param.paramType = static_cast<ShaderParamType>(materialShaderData["Parameters"][i]["Type"]);
			param.paramIndex = materialShaderData["Parameters"][i]["Index"];
			param.offset = materialShaderData["Parameters"][i]["Offset"];
			param.defaultScalarValue = materialShaderData["Parameters"][i]["ScalarValue"];
			param.defaultVectorValue.x = materialShaderData["Parameters"][i]["VectorValue"][0];
			param.defaultVectorValue.y = materialShaderData["Parameters"][i]["VectorValue"][1];
			param.defaultVectorValue.z = materialShaderData["Parameters"][i]["VectorValue"][2];
			param.defaultVectorValue.w = materialShaderData["Parameters"][i]["VectorValue"][3];
			materialShader.ShaderParameters.push_back(param);
		}

		MaterialShaderImportData.insert({ it->first, materialShader });
	}

}

void AssetManager::LoadMaterialAssets()
{
	for (std::map<std::string, std::string>::iterator it = MaterialAssetPaths.begin(); it != MaterialAssetPaths.end(); it++)
	{
		std::ifstream f(it->second);
		nlohmann::json  materialImport = nlohmann::json::parse(f);
		nlohmann::json materialAssetData = materialImport["Material"];
		
		MaterialAssetData matData;
		
		matData.Name = materialAssetData["Name"];
		
		int textureParamSize = materialAssetData["Textures"].size();
		for (int i = 0; i < textureParamSize; i++)
		{
			TexParam tParam;
			tParam.paramName = materialAssetData["Textures"][i]["ParameterName"];
			std::string texName = materialAssetData["Textures"][i]["TextureName"];
			tParam.texture = &TextureMap[texName];
			tParam.IsRenderTexture = false;
			tParam.bindSlot = materialAssetData["Textures"][i]["BindSlot"];
			matData.textureParams.push_back(tParam);
		}

		int paramSize = materialAssetData["Parameters"].size();
		for (int i = 0; i < paramSize; i++)
		{
			ShaderParam param;
			param.paramName = materialAssetData["Parameters"][i]["ParamName"];
			param.type = static_cast<ShaderParamType>(materialAssetData["Parameters"][i]["Type"]);
			param.offset = materialAssetData["Parameters"][i]["Offset"];
			param.valueF = materialAssetData["Parameters"][i]["ScalarValue"];
			param.valueV.x = materialAssetData["Parameters"][i]["VectorValue"][0];
			param.valueV.y = materialAssetData["Parameters"][i]["VectorValue"][1];
			param.valueV.z = materialAssetData["Parameters"][i]["VectorValue"][2];
			param.valueV.w = materialAssetData["Parameters"][i]["VectorValue"][3];
			matData.parameters.push_back(param);
			matData.parametersMap.insert({ param.paramName, param });
		}

		matData.blend = static_cast<BlendMode>(materialAssetData["Blend"]);
		matData.cull = static_cast<CullMode>(materialAssetData["Cull"]);
		matData.fill = static_cast<FillMode>(materialAssetData["Fill"]);

		std::string vsName = materialAssetData["Shader"]["Vertex"];
		std::string psName = materialAssetData["Shader"]["Pixel"];
		matData.vs = &VertexShaderMap[vsName];
		matData.ps = &PixelShaderMap[psName];
		matData.ms = &MaterialShaderImportData[materialAssetData["Shader"]["MaterialShader"]];
		MaterialImportData.insert({it->first, matData});

	}

	for (std::map<std::string, MaterialAssetData>::iterator it = MaterialImportData.begin(); it != MaterialImportData.end(); it++)
	{
		MaterialAssetMap.insert({it->first, MaterialAsset()});
		MaterialAssetMap[it->first].CreateMaterial(renderer, &it->second);
		/*
		MaterialAssetMap[it->first].CreateMaterial(renderer, it->second.Name, it->second.vs, it->second.ps, it->second.parameters,
			it->second.textureParams, it->second.blend,
			it->second.cull, it->second.fill);
		*/
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

AssetManager::~AssetManager()
{
}


bool AssetManager::LoadAllAssets()
{
	LoadAssetPaths();
	LoadTextureAssets();
	LoadMeshAssets();
	LoadShaderAssets();
	LoadMaterialAssets();
	return false;
}

