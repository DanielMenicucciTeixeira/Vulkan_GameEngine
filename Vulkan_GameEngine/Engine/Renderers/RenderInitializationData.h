#ifndef RENDERINITIALIZATIONDATA_H
#define RENDERINITIALIZATIONDATA_H

#include <unordered_map>
#include <vector>
#include <set>
#include "RenderObject.h"
#include "Objects/GameObjects/GameObject.h"
#include "Objects/Components/StaticMeshComponent.h"

struct S_LightInfo;

struct S_RenderData
{
	std::unordered_map<S_Mesh*, std::set<S_Material*>> MeshToMaterialMap;
	std::unordered_map<S_Material*, std::vector<FMatrix4*>> MaterialToModelMap;
	std::set<FMatrix4*> Models;
	std::set<S_Texture*> Textures;
	std::set<S_LightInfo*> LightSources;
	UniformCameraObject* Camera;

	void LoadGameObject(O_GameObject* gameObject)
	{
		for (const auto& staticMesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
		{
			MeshToMaterialMap[staticMesh->GetMesh()].insert(staticMesh->GetMaterial());
			MaterialToModelMap[staticMesh->GetMaterial()].push_back(staticMesh->GetModelMatrix());
			Models.insert(staticMesh->GetModelMatrix());
			if (staticMesh->GetTextureDifuse()) Textures.insert(staticMesh->GetTextureDifuse());
			if (staticMesh->GetTextureSpecular()) Textures.insert(staticMesh->GetTextureSpecular());
		}
	}
};
#endif // !RENDERINITIALIZATIONDATA_H

