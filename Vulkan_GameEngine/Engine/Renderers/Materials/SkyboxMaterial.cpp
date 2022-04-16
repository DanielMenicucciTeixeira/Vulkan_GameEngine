#include "SkyboxMaterial.h"

M_SkyboxMaterial::M_SkyboxMaterial()
{
    Shader.Name = "Skybox";
    Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::VERTEX_BIT, "SkyboxShaderVert"));
    Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::FRAGMENT_BIT, "SkyboxShaderFrag"));
    InstanceName = "M_Skybox";

    CubeMap.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
    CubeMap.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
    CubeMap.Info.Subtype = E_SharderVariableSubtype::COMBINED_IMAGE_SAMPLER_CUBE;
}

std::vector<void*> M_SkyboxMaterial::GetShaderVariablesData() const
{
    return { CubeMap.Value };
}

std::vector<ShaderVariableInfo> M_SkyboxMaterial::GetShaderVariablesInfo() const
{
    return { CubeMap.Info };
}
