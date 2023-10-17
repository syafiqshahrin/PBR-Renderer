#include "MaterialEditor.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "AssetManager.h"
#include <vector>

MaterialEditor::MaterialEditor()
{
    GenerateMaterialList();
    ChangeSelectedMaterial(0);
}

MaterialEditor::~MaterialEditor()
{

}

void MaterialEditor::RenderEditorWindow()
{
    if (ImGui::Begin("Material Editor"))
    {
        RenderMaterialDropDown();

        ImGui::End();
    }
    else
    {
        ImGui::End();
    }
}

void MaterialEditor::GenerateMaterialList()
{
	AssetManager::GetAssetManager()->GetAllLoadedMaterialNames(MaterialList);
}

void MaterialEditor::ChangeSelectedMaterial(UINT index)
{
	CurrentMaterialIndex = index;
	CurrentMaterial = AssetManager::GetAssetManager()->GetAsset<MaterialAsset>(MaterialList[index]);
	CurrentMaterialData = AssetManager::GetAssetManager()->GetAsset<MaterialAssetData>(MaterialList[index]);
}

void MaterialEditor::RenderMaterialDropDown()
{
    std::vector<const char*> mats;
    for (int i = 0; i < MaterialList.size(); i++)
    {
        mats.push_back(MaterialList[i].c_str());
    }
    //const char* DisplayString = mats[CurrentMaterialIndex];
    ImGui::Combo("##Material", &CurrentMaterialIndex, mats.data(), mats.size());
}
