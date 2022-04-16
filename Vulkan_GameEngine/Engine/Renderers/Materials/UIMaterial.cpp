#include "UIMaterial.h"

M_UI_ImageMaterial::M_UI_ImageMaterial() : M_UI_Material()
{
	Shader.Name = "UIShader";
	Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::VERTEX_BIT, "UIShaderVert"));
	Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::FRAGMENT_BIT, "UIShaderFrag"));
	InstanceName = "Image_Material";

	Image.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	Image.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
	Image.Info.VariableSize = sizeof(S_Texture*);
}

M_UI_ImageMaterial::~M_UI_ImageMaterial()
{
}


std::vector<void*>  M_UI_ImageMaterial::GetShaderVariablesData() const
{
	return { Image.Value };
}

std::vector<ShaderVariableInfo>  M_UI_ImageMaterial::GetShaderVariablesInfo() const
{
	return { Image.Info };
}
