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

class MaterialAssetData;
class MaterialAsset;


class MaterialEditor
{
public:
	MaterialEditor();
	~MaterialEditor();
	void RenderEditorWindow();

private:

	std::vector<std::string> MaterialList;
	std::vector<std::string> MaterialTextureList;
	std::vector<std::string> MaterialTextureParamList;
	std::vector<const char*> MaterialListChar;
	int CurrentMaterialIndex;
	MaterialAsset* CurrentMaterial;
	MaterialAssetData* CurrentMaterialData;

	void GenerateMaterialList();
	void GenerateMaterialTextureList();
	void ChangeSelectedMaterial(UINT index);



	void RenderMaterialDropDown();
	void RenderMaterialInfo();
	void RenderTextureParameters();
	void RenderShaderParameters();

	void SaveToMaterialData();
};