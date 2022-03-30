#ifndef VULKANSKYBOXMATERIAL_H
#define VULKANSKYBOXMATERIAL_H

#include "VulkanMaterial.h"
struct S_CubeSampler;

class MV_SkyboxMaterial : public M_VulkanMaterial
{
public:
	MV_SkyboxMaterial();
	virtual S_GraphicsPipelineStagesInfo GetGraphicsPipelineStages() override;

	ShaderVariable<S_CubeSampler*> CubeMap;

	virtual std::vector<void*> GetShaderVariablesData() const override;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const override;
};

#endif

