#include "MaterialEditor.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "AssetManager.h"
#include <vector>
#include "Vector.h"

MaterialEditor::MaterialEditor()
{
    GenerateMaterialShaderList();
    GenerateMaterialList();
    ChangeSelectedMaterial(0);
    ChangeSelectedMaterialShader(0);
    GenerateMaterialTextureList();
    GenerateMaterialShaderParamList();
}

MaterialEditor::~MaterialEditor()
{

}

void MaterialEditor::RenderEditorWindow()
{
    bool open = true;
    if (ImGui::Begin("Material Editor", &open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Create..."))
            {
                if (ImGui::MenuItem("New Material", NULL))
                {
                    ShowMaterialCreationWindow = true;
                    //ImGui::OpenPopup("Delete?");
                    DEBUG("Creating new material");
                }
                ImGui::MenuItem("Duplicate Material", NULL);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

      

        RenderMaterialDropDown();
        RenderTextureParameters();
        RenderShaderParameters();
        RenderStateParameters();
        if (ImGui::Button("Save Changes"))
        {
            SaveToMaterialData();
            AssetManager::GetAssetManager()->SaveAsset<MaterialAssetData>(MaterialList[CurrentMaterialIndex]);
        }
        ImGui::End();
    }
    else
    {
        ImGui::End();
    }

    if (ShowMaterialCreationWindow)
    {
        ImGui::OpenPopup("Create Material");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Create Material", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        RenderMaterialCreatorWindow();
      
        ImGui::EndPopup();
    }

}

void MaterialEditor::GenerateMaterialShaderList()
{
    AssetManager::GetAssetManager()->GetAllLoadedMaterialShaderNames(MaterialShaderList);
    for (int i = 0; i < MaterialShaderList.size(); i++)
    {
        MaterialShaderListChar.push_back(MaterialShaderList[i].c_str());
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

void MaterialEditor::GenerateMaterialShaderParamList()
{
    MaterialShaderParamList.clear();
    CurrentMaterial->GetShaderParamNames(MaterialShaderParamList);
}

void MaterialEditor::ChangeSelectedMaterial(UINT index)
{
	CurrentMaterialIndex = index;
	CurrentMaterial = AssetManager::GetAssetManager()->GetAsset<MaterialAsset>(MaterialList[index]);
	CurrentMaterialData = AssetManager::GetAssetManager()->GetAsset<MaterialAssetData>(MaterialList[index]);
    GenerateMaterialTextureList();
    GenerateMaterialShaderParamList();
}

void MaterialEditor::ChangeSelectedMaterialShader(UINT index)
{
    CurrentMaterialShaderIndex = index;
    CurrentMaterialShaderData = AssetManager::GetAssetManager()->GetAsset<MaterialShader>(MaterialShaderList[index]);
}

void MaterialEditor::RenderMaterialShaderDropdown()
{
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::Combo("##MaterialShader", &CurrentMaterialShaderIndex, MaterialShaderListChar.data(), MaterialShaderListChar.size());
}

void MaterialEditor::RenderMaterialDropDown()
{
    //ImGui::PushItemWidth(-FLT_MIN);
    ImGui::SetNextItemWidth(-FLT_MIN);
    if (ImGui::Combo("##Material", &CurrentMaterialIndex, MaterialListChar.data(), MaterialListChar.size()))
    {
        ChangeSelectedMaterial(CurrentMaterialIndex);
    }
}

void MaterialEditor::RenderTextureParameters()
{
    static int currentIndex;

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("TextureParam", ImVec2(0, 64 * 3.8), true);

    ImGui::Text("Textures");
    for (int i = 0; i < MaterialTextureParamList.size(); i++)
    {
        Texture2D* tex = AssetManager::GetAssetManager()->GetAsset<Texture2D>(MaterialTextureList[i]);
        if (ImGui::ImageButton(tex->GetSRV().Get(), ImVec2(64, 64)))
        {
            currentIndex = i;
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

void MaterialEditor::RenderShaderParameters()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ShaderParam", ImVec2(0, MaterialShaderParamList.size() * 50), true);

    ImGui::Text("Shader Parameters");

    for (int i = 0; i < MaterialShaderParamList.size(); i++)
    {

        ImGui::Text(MaterialShaderParamList[i].c_str());
        ImGui::SameLine(100);
        if (CurrentMaterialData->parametersMap[MaterialShaderParamList[i]].type == ShaderParamType::SCALAR)
        {
            float val = CurrentMaterial->GetScalarParam(MaterialShaderParamList[i]);
            ImGui::DragFloat("##", &val, 0.1f);
            CurrentMaterial->SetScalarParam(MaterialShaderParamList[i], val);
        }
        else
        {
            Vector4 val = CurrentMaterial->GetVectorParam(MaterialShaderParamList[i]);
            float v[4] = { val.x, val.y, val.z, val.w };
            ImGui::DragFloat4("##", v, 0.1f);
            CurrentMaterial->SetVectorParam(MaterialShaderParamList[i], Vector4(v[0], v[1], v[2], v[3]));
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void MaterialEditor::RenderStateParameters()
{
    const char* CullModes[3] = { "Back", "Front", "Two Sided" };
    const char* FillModes[2] = { "Solid", "Wireframe"};
    const char* BlendModes[4] = { "Opaque", "Transparent", "Additive", "Multiply"};

    int currentCullMode = static_cast<int>(CurrentMaterial->GetCullMode());
    int currentFillMode = static_cast<int>(CurrentMaterial->GetFillMode());
    int currentBlendMode = static_cast<int>(CurrentMaterial->GetBlendMode());

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Render States", ImVec2(0, 3 * 40), true);

    ImGui::Text("Render States");
    ImGui::Text("Cull Mode");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::Combo("##Cull", &currentCullMode, CullModes, 3);

    ImGui::Text("Fill Mode");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::Combo("##Fill", &currentFillMode, FillModes, 2);

    ImGui::Text("Blend Mode");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::Combo("##Blend", &currentBlendMode, BlendModes, 4);

    ImGui::EndChild();
    ImGui::PopStyleVar();

    CurrentMaterial->SetCullMode(static_cast<CullMode>(currentCullMode));
    CurrentMaterial->SetFillMode(static_cast<FillMode>(currentFillMode));
    CurrentMaterial->SetBlendMode(static_cast<BlendMode>(currentBlendMode));

}

void MaterialEditor::RenderMaterialCreatorWindow()
{
    ImGui::Text("Material Shader");
    ImGui::SameLine(150);
    RenderMaterialDropDown();
    static char textbuffer[40];

    ImGui::Text("Material Name");
    ImGui::SameLine(150);
    ImGui::InputText("##MatName", textbuffer, 40);
    if (ImGui::Button("Create", ImVec2(120, 0)))
    {
        ShowMaterialCreationWindow = false;
        std::string MaterialAssetName(textbuffer);
        AssetManager::GetAssetManager()->CreateMaterialDataAsset(MaterialAssetName, MaterialShaderList[CurrentMaterialShaderIndex]);
        GenerateMaterialList();
        ChangeSelectedMaterial(0);
        ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
        ShowMaterialCreationWindow = false;
        ImGui::CloseCurrentPopup();
    }

}

void MaterialEditor::SaveToMaterialData()
{
    //Update Textures
    for (int i = 0; i < CurrentMaterialData->textureParams.size(); i++)
    {
        CurrentMaterialData->textureParams[i] = CurrentMaterial->GetTextureParam(CurrentMaterialData->textureParams[i].paramName);
    }
    //UpdateShader param values
    for (int i = 0; i < CurrentMaterialData->parameters.size(); i++)
    {
        if (CurrentMaterialData->parameters[i].type == ShaderParamType::SCALAR)
        {
            CurrentMaterialData->parameters[i].valueF = CurrentMaterial->GetScalarParam(CurrentMaterialData->parameters[i].paramName);
        }
        else
        {
            CurrentMaterialData->parameters[i].valueV = CurrentMaterial->GetVectorParam(CurrentMaterialData->parameters[i].paramName);
        }
    }
    //Update Render states
    CurrentMaterialData->cull = CurrentMaterial->GetCullMode();
    CurrentMaterialData->fill = CurrentMaterial->GetFillMode();
    CurrentMaterialData->blend = CurrentMaterial->GetBlendMode();
}

