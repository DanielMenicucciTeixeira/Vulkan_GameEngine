#ifndef SKYBOXMATERIAL_H
#define SKYBOXMATERIAL_H

#include "Material.h"
#include"Renderers/RenderObject.h"

class M_SkyboxMaterial :  public Material
{
public:
	M_SkyboxMaterial();
	ShaderVariable<S_CubeSampler*> CubeMap;

	virtual std::vector<void*> GetShaderVariablesData() const override;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const override;
};

#endif // !SKYBOXMATERIAL_H
