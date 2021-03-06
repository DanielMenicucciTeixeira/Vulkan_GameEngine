#include "LevelGraph.h"

#include <typeinfo>
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include "SDL/SDLTextureHandler.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/CollisionComponent.h"

//Static Re-declarations

std::unique_ptr<LevelGraph> LevelGraph::Instance = nullptr;

std::unordered_map<std::string, S_Texture*> LevelGraph::TexturesByName;
std::unordered_map<std::string, S_Material*> LevelGraph::MaterialsByName;
std::unordered_map<std::string, S_Mesh*> LevelGraph::MeshesByName;

C_CameraComponent* LevelGraph::ActiveCamera;
std::set<O_Object*> LevelGraph::UnloadedObjects;
std::map<std::string, O_Object*> LevelGraph::GameObjectsByName;
std::map<std::string, std::set<O_Object*>> LevelGraph::GameObjectsByTag;
std::map<size_t, std::set<O_Object*>> LevelGraph::GameObjectsByClass;
OctSpactilPartition*  LevelGraph::ColliderSpationPartition = new OctSpactilPartition(100.0f);
//-----------------------

LevelGraph* LevelGraph::GetInstance()
{
	if (Instance.get() == nullptr) Instance.reset(new LevelGraph);
	return Instance.get();
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

void LevelGraph::SetActiveCamera(C_CameraComponent* camera)

{
	ActiveCamera = camera;
	RenderData.Camera = camera->GetUCO();
}

void LevelGraph::FrameBufferResizeCallback()
{
	ActiveCamera->UpdateProjection();
}

void LevelGraph::AddMesh(S_Mesh* mesh)
{
	MeshesByName[mesh->Name] = mesh;
	RenderData.Meshes.insert(mesh);
}

void LevelGraph::AddMeshComponent(C_StaticMeshComponent* meshComponent)
{
	if (!RenderData.MaterialsByShader.count(meshComponent->GetMaterial()->ShaderName)) RenderData.MaterialsByShader[meshComponent->GetMaterial()->ShaderName] = std::set<S_Material*>();
	RenderData.MaterialsByShader[meshComponent->GetMaterial()->ShaderName].insert(meshComponent->GetMaterial());

	if (!RenderData.MeshesByMaterial.count(meshComponent->GetMaterial())) RenderData.MeshesByMaterial[meshComponent->GetMaterial()] = std::set<S_Mesh*>();
	RenderData.MeshesByMaterial[meshComponent->GetMaterial()].insert(meshComponent->GetMesh());
	
	if (!RenderData.InstancesByMesh.count(meshComponent->GetMesh())) RenderData.InstancesByMesh[meshComponent->GetMesh()] = std::set<FMatrix4*>();
	RenderData.InstancesByMesh[meshComponent->GetMesh()].insert(meshComponent->GetModelMatrix());

	RenderData.Models[meshComponent->GetModelMatrix()] = meshComponent->IsInFrustum();
	RenderData.Materials.insert(meshComponent->GetMaterial());
	if(meshComponent->GetMaterial()->TextureDifuse) RenderData.Textures.insert(meshComponent->GetMaterial()->TextureDifuse);
	if (meshComponent->GetMaterial()->TextureSpecular) RenderData.Textures.insert(meshComponent->GetMaterial()->TextureSpecular);
	RenderData.Meshes.insert(meshComponent->GetMesh());
	StaticMehes.insert(meshComponent);
}

void LevelGraph::AddCollisionComponent(C_CollisionComponent* component)
{
	ColliderSpationPartition->AddCollider(component);
}

void LevelGraph::RemoveMeshComponent(C_StaticMeshComponent* meshComponent)
{
	RenderData.MaterialsByShader[meshComponent->GetMaterial()->ShaderName].erase(meshComponent->GetMaterial());
	RenderData.MeshesByMaterial[meshComponent->GetMaterial()].erase(meshComponent->GetMesh());
	RenderData.InstancesByMesh[meshComponent->GetMesh()].erase(meshComponent->GetModelMatrix());
}

void LevelGraph::AddTexture(S_Texture* texture)
{
	TexturesByName[texture->Name] = texture;
	RenderData.Textures.insert(texture);
}

void LevelGraph::AddMaterial(S_Material* material)
{
	MaterialsByName[material->Name] = material;
	RenderData.Materials.insert(material);
	LoadMaterial(material);
}

void LevelGraph::RemoveLight(unsigned int index)
{
	RenderData.LightSources[index] = FMatrix4(0);
	FreeLightSlots.insert(index);
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
	
	if (!MaterialsByName.empty())
	{
		for (auto& material : MaterialsByName) if (material.second) delete (material.second);
		MaterialsByName.clear();
	}

	RenderData.Clear();
}

void LevelGraph::LoadMesh()
{
}

void LevelGraph::LoadModel()
{
}

void LevelGraph::LoadMaterial(S_Material* material)
{
	if (material->TextureNameDifuse != "") LoadTexture(material->TextureDifuse, material->TextureNameDifuse);
	if (material->TextureNameSpecular != "") LoadTexture(material->TextureSpecular, material->TextureNameSpecular);
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
		if (!SDLTextureHandler::LoadTexture(textureName, textures[textureName]->Path, textures[textureName]))
		{
			DebugLogger::Error("Failed to load texture: " + texture->Name + " at " + texture->Path, "Core/Level.cpp", __LINE__);
			return false;
		}
	}

	texture = textures[textureName];
	return true;
}

void LevelGraph::GenerateSpationPartition(float worldSize, unsigned int depth)
{
	if(ColliderSpationPartition) delete ColliderSpationPartition;
	ColliderSpationPartition = new OctSpactilPartition(worldSize, depth);
}

std::set<OctSpactilPartition::OctNode*> LevelGraph::GetIntersectedLeaves(Ray& ray) const
{
	return ColliderSpationPartition->GetIntersectedLeaves(ray);
}

LevelGraph::LevelGraph()
{
	RenderData.LightSources.reserve(30);
	FreeLightSlots = std::set<unsigned int>();
}

LevelGraph::~LevelGraph()
{
	CleanUp();
}
