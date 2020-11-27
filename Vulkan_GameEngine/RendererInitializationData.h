#ifndef RENDERERINITIALIZATIONDATA_H
#define RENDERERINITIALIZATIONDATA_H

#include <unordered_map>
#include <vector>
#include "RenderObject.h"
#include "UniformCameraObject.h"



struct RendererInitializationData
{
	std::unordered_map<MeshStruct*, std::vector<TextureStruct*>> MeshToMaterialMap;
	std::unordered_map<TextureStruct*, std::vector<UniformBufferObject*>> MaterialToUBOMap;
	std::vector<UniformBufferObject*> UBOs;
};

#endif
