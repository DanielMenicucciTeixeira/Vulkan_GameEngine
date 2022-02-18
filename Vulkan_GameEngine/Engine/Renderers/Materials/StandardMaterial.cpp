#include "StandardMaterial.h"

M_StandardMaterial::M_StandardMaterial()
{
	ShaderName = "TextureShader";
	MaterialName = "StandardMaterial";

	ReflectionData.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	ReflectionData.Info.Type = E_ShaderVariableType::UNIFORM_BUFFER;

	DiffuseTexture.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	DiffuseTexture.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;

	SpecularTexture.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
	SpecularTexture.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
}

M_StandardMaterial::~M_StandardMaterial()
{
}

std::vector<void*> M_StandardMaterial::GetShaderVariablesData()
{
	return { &ReflectionData.Value, DiffuseTexture.Value->Pixels, SpecularTexture.Value->Pixels };
}

std::vector<ShaderVariableInfo> M_StandardMaterial::GetShaderVariablesInfo()
{
	return { ReflectionData.Info, DiffuseTexture.Info, SpecularTexture.Info };
}
