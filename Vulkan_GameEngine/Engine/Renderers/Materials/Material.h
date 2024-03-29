#ifndef MATERIAL_H
#define MATERIAL_H

#include "Math/FMatrix4.h"
#include "Renderers/RenderObject.h"
#include <string>
#include <vector>

enum class E_ShaderVariableType
{
	SAMPLER = 0,
	COMBINED_IMAGE_SAMPLER = 1,
	SAMPLED_IMAGE = 2,
	STORAGE_IMAGE = 3,
	UNIFORM_TEXEL_BUFFER = 4,
	STORAGE_TEXEL_BUFFER = 5,
	UNIFORM_BUFFER = 6,
	STORAGE_BUFFER = 7,
	UNIFORM_BUFFER_DYNAMIC = 8,
	STORAGE_BUFFER_DYNAMIC = 9,
	INPUT_ATTACHMENT = 10,
	INLINE_UNIFORM_BLOCK_EXT = 1000138000,
	ACCELERATION_STRUCTURE_KHR = 1000165000,
	ACCELERATION_STRUCTURE_NV = ACCELERATION_STRUCTURE_KHR,
	MAX_ENUM = 0x7FFFFFFF
};

enum class E_SharderVariableSubtype
{
	NONE = 0,
	COMBINED_IMAGE_SAMPLER_TEXTURE = 1,
	COMBINED_IMAGE_SAMPLER_CUBE = 2,
	ALL_SUBTYPES
};

enum class E_ShaderStage
{
	VERTEX_BIT = 0x00000001,
	TESSELLATION_CONTROL_BIT = 0x00000002,
	TESSELLATION_EVALUATION_BIT = 0x00000004,
	GEOMETRY_BIT = 0x00000008,
	FRAGMENT_BIT = 0x00000010,
	COMPUTE_BIT = 0x00000020,
	ALL_GRAPHICS = 0x0000001F,
	ALL = 0x7FFFFFFF,

	//Vulkan Specific Stages

	// Provided by VK_KHR_ray_tracing_pipeline
	VK_RAYGEN_BIT_KHR = 0x00000100,
	// Provided by VK_KHR_ray_tracing_pipeline
	VK_ANY_HIT_BIT_KHR = 0x00000200,
	// Provided by VK_KHR_ray_tracing_pipeline
	VK_CLOSEST_HIT_BIT_KHR = 0x00000400,
	// Provided by VK_KHR_ray_tracing_pipeline
	VK_MISS_BIT_KHR = 0x00000800,
	// Provided by VK_KHR_ray_tracing_pipeline
	VK_INTERSECTION_BIT_KHR = 0x00001000,
	// Provided by VK_KHR_ray_tracing_pipeline
	VK_CALLABLE_BIT_KHR = 0x00002000,
	// Provided by VK_NV_mesh_shader
	VK_TASK_BIT_NV = 0x00000040,
	// Provided by VK_NV_mesh_shader
	VK_MESH_BIT_NV = 0x00000080,
	// Provided by VK_HUAWEI_subpass_shading
	VK_SUBPASS_SHADING_BIT_HUAWEI = 0x00004000,
	// Provided by VK_NV_ray_tracing
	VK_RAYGEN_BIT_NV = VK_RAYGEN_BIT_KHR,
	// Provided by VK_NV_ray_tracing
	VK_ANY_HIT_BIT_NV = VK_ANY_HIT_BIT_KHR,
	// Provided by VK_NV_ray_tracing
	VK_CLOSEST_HIT_BIT_NV = VK_CLOSEST_HIT_BIT_KHR,
	// Provided by VK_NV_ray_tracing
	VK_MISS_BIT_NV = VK_MISS_BIT_KHR,
	// Provided by VK_NV_ray_tracing
	VK_INTERSECTION_BIT_NV = VK_INTERSECTION_BIT_KHR,
	// Provided by VK_NV_ray_tracing
	VK_CALLABLE_BIT_NV = VK_CALLABLE_BIT_KHR,
};

struct ShaderVariableInfo
{
	ShaderVariableInfo() {}

	ShaderVariableInfo(E_ShaderStage stage, E_ShaderVariableType type, size_t size = 0) :
		Stage(stage ), Type(type), VariableSize(size) {}

	E_ShaderStage Stage;
	E_ShaderVariableType Type;
	E_SharderVariableSubtype Subtype = E_SharderVariableSubtype::NONE;
	size_t VariableSize;
};

template <typename T>
struct ShaderVariable
{
	ShaderVariable(T value, E_ShaderVariableType type, E_ShaderStage stage, size_t size = 0) : Value(value), Info(stage, type, size) {}
	ShaderVariable() {}

	T Value;
	ShaderVariableInfo Info;

	inline void operator= (const T& value) { Value = value; }

	bool operator== (const ShaderVariable<T>& other)
	{
		return Value == other.Value && Info.Stage == other.Info.Stage && Info.Type == other.Info.Type;
	}
};

struct S_ShaderInfo
{
	std::string Name;
	std::vector<std::pair<E_ShaderStage, std::string>> ShaderFiles;
};

class Material
{
public:
	Material() {}
	virtual ~Material() {}

protected:
	std::string InstanceName;
	S_ShaderInfo Shader;

public:
	inline const std::string& GetMaterialName() const { return InstanceName; }
	inline void SetMaterialName(std::string name) { InstanceName = name; }
	inline const S_ShaderInfo& GetShaderInfo() const { return Shader; }

	virtual std::vector<void*> GetShaderVariablesData() const = 0;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const = 0;
};

#endif