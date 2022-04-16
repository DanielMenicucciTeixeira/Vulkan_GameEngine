#ifndef M_UIMATERIAL_H
#define M_UIMATRIAL_H

#include "Material.h"
#include "VulkanMaterial.h"

struct S_Texture;

class M_UI_Material :  virtual public Material
{
public:
	M_UI_Material() : Material() {}
	virtual ~M_UI_Material() {}
};

class M_UI_ImageMaterial : public M_UI_Material
{
public:
	M_UI_ImageMaterial();
	virtual ~M_UI_ImageMaterial();

	ShaderVariable<S_Texture*> Image;

	virtual std::vector<void*> GetShaderVariablesData() const override;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const override;
};

class M_UI_VulkanMaterial : public M_UI_Material, public M_VulkanMaterial
{
public:
	M_UI_VulkanMaterial() : M_UI_Material() {}
	virtual ~M_UI_VulkanMaterial() {}
};

#endif

