#include "LevelGraph.h"

#include <typeinfo>
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Objects/GameObjects/GO_Camera.h"
#include "CollisionHandler.h"
#include "Renderers/TextureHandler.h"
#include "Objects/Components/UIComponent.h"
#include "Window.h"
#include "CoreEngine.h"
#include "Renderers/Materials/UIMaterial.h"
#include "Graphics/TextureLoader.h"

//Static Re-declarations

std::unique_ptr<LevelGraph> LevelGraph::Instance = nullptr;

std::set<O_Object*> LevelGraph::UnloadedObjects;
std::map<std::string, std::set<O_Object*>> LevelGraph::GameObjectsByTag;
std::map<size_t, std::set<O_Object*>> LevelGraph::GameObjectsByClass;
//-----------------------

LevelGraph* LevelGraph::GetInstance()
{
	if (Instance.get() == nullptr) {
		Instance.reset(new LevelGraph);
	}
	return Instance.get();
}


void LevelGraph::Render()
{
	for (auto& mesh : StaticMehes)
	{
		mesh->SetInFrustum(ActiveCamera->FrustumCheck(mesh->GetBoxCollider()));
	}
}

void LevelGraph::AddObject(O_Object* gameObject)
{
	if (gameObject)
	{
		size_t classID = typeid(*gameObject).hash_code();
		if (!GameObjectsByClass.count(classID)) GameObjectsByClass[classID] = std::set<O_Object*>();
		GameObjectsByClass[classID].insert(gameObject);
		
		for (const auto& tag : gameObject->GetTags())
		{
			if (!GameObjectsByTag.count(tag)) GameObjectsByTag[tag] = std::set<O_Object*>();
			GameObjectsByTag[tag].insert(gameObject);
		}

		std::string name = gameObject->GetName();
		if (gameObject->GetName() == "")
		{
			name = typeid(*gameObject).name();
			name = name.substr(6);//Remove the "class " part of the typeid name
		}

		if (GameObjectsByName.count(name))
		{
			unsigned int i = 1;
			std::string newName;
			do
			{
				newName = name + "(" + std::to_string(i) + ")";
				if (GameObjectsByName.count(newName)) i++;
				else
				{
					name = newName;
					break;
				}
			} while (true);
		}
		GameObjectsByName[name] = gameObject;
		gameObject->SetName(name);
	}
}

void LevelGraph::RemoveObjectFromTagList(O_Object* gameObject, std::string tag)
{
	if (gameObject->Tags.count(tag)) gameObject->Tags.erase(tag);
	if (GameObjectsByTag[tag].count(gameObject)) GameObjectsByTag[tag].erase(gameObject);
}

void LevelGraph::AddObjectToTagList(O_Object* gameObject, std::string tag)
{
	gameObject->Tags.insert(tag);
	GameObjectsByTag[tag].insert(gameObject);
}

void LevelGraph::FrameBufferResizeCallback()
{
	ActiveCamera->UpdateProjection();
	FVector2 windowSize = CoreEngine::GetInstance()->GetWindowSize();
	RenderData.AspectRatio = windowSize.X / windowSize.Y;
}

void LevelGraph::AddMesh(S_Mesh* mesh)
{
	MeshesByName[mesh->Name] = mesh;
}

void LevelGraph::AddMeshComponent(C_StaticMeshComponent* meshComponent)
{
	std::string shaderName = meshComponent->GetMaterial()->GetShaderInfo().Name;
	Material* material = meshComponent->GetMaterial();
	S_Mesh* mesh = meshComponent->GetMesh();
	if (!RenderData.DataMapByShader.count(shaderName)) RenderData.DataMapByShader[shaderName] = MeshesByMaterial_T();
	if (!RenderData.DataMapByShader[shaderName].count(material)) RenderData.DataMapByShader[shaderName][material] = ModelsByMesh_T();
	if (!RenderData.DataMapByShader[shaderName][material].count(mesh)) RenderData.DataMapByShader[shaderName][material][mesh] = std::set<S_ModelData>();
	RenderData.DataMapByShader[shaderName][material][mesh].insert(S_ModelData(meshComponent->GetModelMatrix(), meshComponent->IsInFrustum()));


	RenderData.Models.insert(meshComponent->GetModelMatrix());
	StaticMehes.insert(meshComponent);
}

void LevelGraph::AddCollisionComponent(C_CollisionComponent* component)
{
	//ColliderSpationPartition->AddCollider(component);
}

void LevelGraph::RemoveMeshComponent(C_StaticMeshComponent* meshComponent)
{
	Material* material = meshComponent->GetMaterial();
	RenderData.DataMapByShader[material->GetShaderInfo().Name][material][meshComponent->GetMesh()].erase(meshComponent->GetModelData());
	RenderData.Models.erase(meshComponent->GetModelMatrix());
}

void LevelGraph::RemoveUIElement(UI_Element* element)
{
	M_UI_Material* material = dynamic_cast<M_UI_Material *>(MaterialInstancesByName[element->GetMaterialInstanceName()]);
	if (!material) return;
	RenderData.UIMapByShader[material->GetShaderInfo().Name][material].erase(element->GetModelData());
	RenderData.UIElements.erase(element->GetRect()->GetModel());
}

void LevelGraph::AddUIElement(UI_Element* element)
{
	M_UI_Material* material = dynamic_cast<M_UI_Material*>(MaterialInstancesByName[element->GetMaterialInstanceName()]);
	if (!material) material = dynamic_cast<M_UI_Material*>(MaterialInstancesByName["UIMaterial"]);
	if (!material) return;
	std::string shaderName = material->GetShaderInfo().Name;
	if (!RenderData.UIMapByShader.count(shaderName)) RenderData.UIMapByShader[shaderName] = UIModelsByMaterial_T();
	if (!RenderData.UIMapByShader[shaderName].count(material)) RenderData.UIMapByShader[shaderName][material] = std::set<S_ModelData>();
	RenderData.UIMapByShader[shaderName][material].insert(S_ModelData(element->GetRect()->GetModel(), element->IsVisible()));

	RenderData.UIElements.insert(element->GetRect()->GetModel());
}

void LevelGraph::AddTexture(S_Texture* texture)
{
	TexturesByName[texture->Name] = texture;
	RenderData.Textures.insert(texture);
}

void LevelGraph::AddCubeSampler(S_CubeSampler* sampler)
{
	CubeSamplersByName[sampler->Name] = sampler;
	RenderData.CubeSamplers.insert(sampler);
}

void LevelGraph::AddMaterial(Material* material)
{
	MaterialInstancesByName[material->GetMaterialName()] = material;
	//RenderData.Materials.insert(material);
	LoadMaterial(material);
}

void LevelGraph::RemoveLight(unsigned int index)
{
	RenderData.LightSources[index] = FMatrix4(0);
	FreeLightSlots.insert(index);
}

void LevelGraph::AddCamera(C_CameraComponent* cam, std::string)
{
	if (CameraList.empty()) {
		SetActiveCamera(cam);
	}
	CameraList.push_back(cam);
}

void LevelGraph::SetActiveCamera(C_CameraComponent* camera)
{
	if (!camera)
	{
		DebugLogger::Error("No valid Camera found!", "Core/LevelGraph.cpp", __LINE__);
		return;
	}
	ActiveCamera = camera;
	RenderData.Camera = camera->GetUCO();
}

std::vector<C_CameraComponent*> * LevelGraph::GetAllCameras()
{
	return &CameraList;
}

C_CameraComponent * LevelGraph::GetCamera(int cameraID)
{
	return CameraList[cameraID];
}

void LevelGraph::AddLight(FMatrix4*& matrix, unsigned int& index)
{
	if (FreeLightSlots.empty())
	{
		RenderData.LightSources.push_back(FMatrix4());
		index = RenderData.LightSources.size() - 1;
		matrix = &RenderData.LightSources[index];
		int breakPoint = 0;
	}
	else
	{
		index = *FreeLightSlots.begin();
		FreeLightSlots.erase(index);
		matrix = &RenderData.LightSources[index];
	}
}

//TODO: This needs to get called somewhere.
void LevelGraph::CleanUp()
{
	if (!GameObjectsByName.empty())
	{
		for (auto& object : GameObjectsByName) if (object.second) delete (object.second);
		GameObjectsByName.clear();
	}
	
	if (!MeshesByName.empty())
	{
		for (auto& mesh : MeshesByName)  if (mesh.second) delete (mesh.second);
		MeshesByName.clear();
	}
	
	if (!MaterialInstancesByName.empty())
	{
		for (auto& material : MaterialInstancesByName) if (material.second) delete (material.second);
		MaterialInstancesByName.clear();
	}

	if (!TexturesByName.empty())
	{
		for (auto& texture : TexturesByName) if (texture.second) delete (texture.second);
		TexturesByName.clear();
	}

	if (!CubeSamplersByName.empty())
	{
		for (auto& cubeSampler : CubeSamplersByName) if (cubeSampler.second) delete (cubeSampler.second);
		CubeSamplersByName.clear();
	}

	RenderData.Clear();
}

void LevelGraph::Pause()
{
	if (isPaused) { isPaused = false; }
	else { isPaused = true; }
}

void LevelGraph::LoadMesh()
{
}

void LevelGraph::LoadModel()
{
}

void LevelGraph::LoadMaterial(Material* material)
{
	//if (material->TextureNameDifuse != "") LoadTexture(material->TextureDifuse, material->TextureNameDifuse);
	//if (material->TextureNameSpecular != "") LoadTexture(material->TextureSpecular, material->TextureNameSpecular);
}

bool LevelGraph::LoadTexture(S_Texture*& texture, const std::string& textureName)
{
	auto& textures = LevelGraph::GetInstance()->GetTextures();
	if (!textures[textureName])
	{
		DebugLogger::Error("Failed to find texture with name: " + texture->Name + ". ", "Core/Level.cpp", __LINE__);
		return false;
	}

	if (!textures[textureName]->Pixels)
	{
		if (!TextureHandler::LoadTexture(textureName, textures[textureName]->Path, textures[textureName]))
		{
			DebugLogger::Error("Failed to load texture: " + texture->Name + " at " + texture->Path, "Core/Level.cpp", __LINE__);
			return false;
		}
	}

	texture = textures[textureName];
	return true;
}

LevelGraph::LevelGraph()
{
	RenderData.LightSources.reserve(30);
	FreeLightSlots = std::set<unsigned int>();
	FVector2 windowSize = CoreEngine::GetInstance()->GetWindowSize();
	RenderData.AspectRatio = windowSize.X / windowSize.Y;
}

LevelGraph::~LevelGraph()
{
	CleanUp();
}

std::ostream& operator<<(std::ostream& out, const O_Object& obj)
{
	out << obj << std::endl;
	return out;
}
