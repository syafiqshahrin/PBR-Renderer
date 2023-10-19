#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"
#include <set>
#include <string>
#include <vector>

struct MaterialAssetData;
class MaterialAsset;
struct MaterialShader;

class MaterialEditor
{
public:
	MaterialEditor();
	~MaterialEditor();
	void RenderEditorWindow();
	
	//Temporary
	void SetDisplayedMaterial(MaterialAsset** mat);

private:
	//Temp
	MaterialAsset** DisplayedMaterial;
	
	std::vector<std::string> MaterialShaderList;
	std::vector<const char*> MaterialShaderListChar;
	std::vector<std::string> MaterialList;
	std::vector<std::string> MaterialTextureList;
	std::vector<std::string> MaterialTextureParamList;
	std::vector<std::string> MaterialShaderParamList;
	std::vector<const char*> MaterialListChar;
	int CurrentMaterialIndex;
	int CurrentMaterialShaderIndex;
	MaterialAsset* CurrentMaterial;
	MaterialAssetData* CurrentMaterialData;
	MaterialShader* CurrentMaterialShaderData;
	bool ShowMaterialCreationWindow;

	void GenerateMaterialShaderList();
	void GenerateMaterialList();
	void GenerateMaterialTextureList();
	void GenerateMaterialShaderParamList();
	void ChangeSelectedMaterial(UINT index);
	void ChangeSelectedMaterialShader(UINT index);


	void RenderMaterialShaderDropdown();
	void RenderMaterialDropDown();
	void RenderMaterialInfo();
	void RenderTextureParameters();
	void RenderShaderParameters();
	void RenderStateParameters();
	void RenderMaterialCreatorWindow();

	void SaveToMaterialData();
};