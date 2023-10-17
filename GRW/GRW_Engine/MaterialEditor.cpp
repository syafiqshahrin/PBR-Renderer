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
    GenerateMaterialTextureList();
}

MaterialEditor::~MaterialEditor()
{

}

void MaterialEditor::RenderEditorWindow()
{
    if (ImGui::Begin("Material Editor"))
    {
        RenderMaterialDropDown();
        RenderTextureParameters();
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
    for (int i = 0; i < MaterialList.size(); i++)
    {
        MaterialListChar.push_back(MaterialList[i].c_str());
    }
}

void MaterialEditor::GenerateMaterialTextureList()
{
    MaterialTextureList.clear();
    MaterialTextureParamList.clear();
    CurrentMaterial->GetTextureNames(MaterialTextureList);
    CurrentMaterial->GetTextureParamNames(MaterialTextureParamList);
}

void MaterialEditor::ChangeSelectedMaterial(UINT index)
{
	CurrentMaterialIndex = index;
	CurrentMaterial = AssetManager::GetAssetManager()->GetAsset<MaterialAsset>(MaterialList[index]);
	CurrentMaterialData = AssetManager::GetAssetManager()->GetAsset<MaterialAssetData>(MaterialList[index]);
    GenerateMaterialTextureList();
}

void MaterialEditor::RenderMaterialDropDown()
{
    //ImGui::PushItemWidth(-FLT_MIN);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::Combo("##Material", &CurrentMaterialIndex, MaterialListChar.data(), MaterialListChar.size());
}

void MaterialEditor::RenderTextureParameters()
{
    static int currentIndex;

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildR", ImVec2(0, 64 * 3.8), true);
    for (int i = 0; i < MaterialTextureParamList.size(); i++)
    {
        Texture2D* tex = AssetManager::GetAssetManager()->GetAsset<Texture2D>(MaterialTextureList[i]);
        if (ImGui::ImageButton(tex->GetSRV().Get(), ImVec2(64, 64)))
        {
            currentIndex = i;
            DEBUG(currentIndex);
            ImGui::OpenPopup("my_select_popup");
        }

        ImGui::SetItemTooltip(MaterialTextureList[i].c_str());
        ImGui::SameLine();
        ImGui::Text(MaterialTextureParamList[i].c_str());
    }
   

    if (ImGui::BeginPopup("my_select_popup"))
    {
        DEBUG("Loop - " << currentIndex);
        std::vector<std::string> LoadedTextures;
        AssetManager::GetAssetManager()->GetAllLoadedTextureNames(LoadedTextures);
        std::vector<std::string>::iterator it = std::find(LoadedTextures.begin(), LoadedTextures.end(), MaterialTextureList[currentIndex]);
        int texIndex = (it - LoadedTextures.begin());
        ImGui::SeparatorText(MaterialTextureList[currentIndex].c_str());
        for (int j = 0; j < LoadedTextures.size(); j++)
        {
            if (ImGui::Selectable(LoadedTextures[j].c_str()))
            {
                Texture2D* newTex = AssetManager::GetAssetManager()->GetAsset<Texture2D>(LoadedTextures[j]);
                CurrentMaterial->SetTexture(MaterialTextureParamList[currentIndex], newTex, CurrentMaterial->GetTextureParamBindSlot(MaterialTextureParamList[currentIndex]), false);
                GenerateMaterialTextureList();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}
