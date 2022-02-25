#include "StandardMaterial.h"

M_StandardMaterial::M_StandardMaterial()
{
	ShaderName = "TextureShader";
	MaterialName = "StandardMaterial";

	ReflectionData.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	ReflectionData.Info.Type = E_ShaderVariableType::UNIFORM_BUFFER;
	ReflectionData.Info.VariableSize = sizeof(FMatrix4);

	DiffuseTexture.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	DiffuseTexture.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
	DiffuseTexture.Info.VariableSize = sizeof(S_Texture*);

	SpecularTexture.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	SpecularTexture.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
	SpecularTexture.Info.VariableSize = sizeof(S_Texture*);
}

M_StandardMaterial::~M_StandardMaterial()
{
}

std::vector<void*> M_StandardMaterial::GetShaderVariablesData() const
{
	return { (void*)&ReflectionData.Value, DiffuseTexture.Value, SpecularTexture.Value };
}

std::vector<ShaderVariableInfo> M_StandardMaterial::GetShaderVariablesInfo() const
{
	return { ReflectionData.Info, DiffuseTexture.Info, SpecularTexture.Info };
}
