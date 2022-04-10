#ifndef STANDARDMATERIAL_H
#define STANDARDMATERIAL_H

#include "Material.h"

class M_StandardMaterial : public Material
{
public:
	M_StandardMaterial();
	virtual ~M_StandardMaterial();

	ShaderVariable<FMatrix4> ReflectionData;
	ShaderVariable<S_Texture*> DiffuseTexture;
	ShaderVariable<S_Texture*> SpecularTexture;

	virtual std::vector<void*> GetShaderVariablesData() const override;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const override;

protected:
	void SetReflectionDataDefaults();
};

#endif
