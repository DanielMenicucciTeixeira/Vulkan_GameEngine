#include "Level.h"
#include "Objects/Components/CollisionComponent.h"
#include "Game.h"
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "Objects/Components/CameraComponent.h"
#include "Renderers/Renderer.h"
#include "SDL/SDLTextureHandler.h"

O_Level::O_Level() : O_Object(), CurrentGame(nullptr), RenderData(nullptr), CurrentCamera(nullptr)
{
	RenderData = new S_RenderData();
}

O_Level::~O_Level()
{
}

bool O_Level::Initialize(Game* game)
{
	if (!game) return false;
	for (auto& mesh : Meshes) LoadMesh(mesh.second);
	for (auto& material : Materials) LoadMaterial(material.second);
	CurrentGame = game;
	LoadLevelObjects();
	CurrentGame->GetRenderer()->Initialize(RenderData);
	return true;
}

void O_Level::Start()
{
	ReloadLevelObjects();
	for (const auto& object : LevelObjects) object->Start();
	CheckForCamera();
	LoadCamera();
}

void O_Level::LoadMesh(S_Mesh* mesh)
{
	if (!Meshes[mesh->Name]) Meshes[mesh->Name] = mesh;
	MeshLoader::LoadMesh(mesh->Path, mesh);
}

S_Mesh* O_Level::GetMesh(std::string meshName)
{
	return Meshes[meshName];
}

void O_Level::LoadMaterial(S_Material* material)
{
	if (!Materials[material->Name]) Materials[material->Name] = material;
	if (material->TextureNameDifuse != "") LoadTexture(material->TextureDifuse, material->TextureNameDifuse);
	if (material->TextureNameSpecular != "") LoadTexture(material->TextureSpecular, material->TextureNameSpecular);
}

bool O_Level::LoadTexture(S_Texture*& texture, const std::string& textureName)
{
	if (!Textures[textureName])
	{
		DebugLogger::Error("Failed to find texture with name: " + texture->Name + ". ", "Core/Level.cpp", __LINE__);
		return false;
	}

	if (!Textures[textureName]->Pixels)
	{
		if (!SDLTextureHandler::LoadTexture(textureName, Textures[textureName]->Path, Textures[textureName]))
		{
			DebugLogger::Error("Failed to load texture: " + texture->Name + " at " + texture->Path, "Core/Level.cpp", __LINE__);
			return false;
		}
	}
	
	texture = Textures[textureName];
	return true;
}

S_Material* O_Level::GetMaterial(std::string materialName)
{
	return Materials[materialName];
}

void O_Level::LoadLevelObjects()
{
	for (auto& object : UnloadedObjects)
	{
		O_GameObject* gameObject = dynamic_cast<O_GameObject*>(object);
		if (gameObject)
		{
			for (auto& mesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
			{
				auto check = Materials[mesh->GetMaterialName()];
				mesh->SetMaterial(Materials[mesh->GetMaterialName()]);
				mesh->SetMesh(Meshes[mesh->GetMeshName()]);
			}
			RenderData->LoadGameObject(dynamic_cast<O_GameObject*>(object));
		}
		LevelObjects.insert(object);
	}

	if (UnloadedObjects.size() > 0)
	{
		UnloadedObjects = std::set<O_Object*>();
	}
}

void O_Level::ReloadLevelObjects()
{
	LoadLevelObjects();
	CurrentGame->GetRenderer()->UpdateWithNewObjects();
}

bool O_Level::LoadCamera()
{
	if (!CurrentCamera)
	{
		DebugLogger::FatalError("No valid CameraFound! RenderData incomplete!", "Core/Level.cpp", __LINE__);
		return false;
	}
	RenderData->Camera = CurrentCamera->GetUCO();
}

bool O_Level::FindAnyCamera()
{
	for (const auto gameObject : LevelObjects)
	{
		if (dynamic_cast<O_GameObject*>(gameObject))
		{
			for (const auto camera : dynamic_cast<O_GameObject*>(gameObject)->GetComponentsOfClass<C_CameraComponent>())
			{
				CurrentCamera = camera;
				return true;
			}
		}
	}
	DebugLogger::Warning("No camera found!", "Core/Level.cpp", __LINE__);
	return false;
}

void O_Level::ChangeCamera()
{
	CurrentCamera = NextCamera;
	LoadCamera();
	ShouldChangeCamera = false;
}

bool O_Level::CheckForCamera()
{
	if (ShouldChangeCamera) ChangeCamera();
	if (CurrentCamera || FindAnyCamera()) return true;
	else
	{
		DebugLogger::FatalError("No valid camera found!", "Core/Level.cpp", __LINE__);
		return false;
	}
}

void O_Level::Update(const float deltaTime)
{
	ReloadLevelObjects();
	if (!CurrentGame->IsPaused()) for (const auto& object : LevelObjects) object->Update(deltaTime);
	else for (const auto& object : LevelObjects) if (object->UpdateWhenPaused) object->Update(deltaTime);
	C_CollisionComponent::CheckForCollisions(Colliders);
}

void O_Level::Render()
{
	CurrentGame->GetRenderer()->Render();
}

void O_Level::CleanUp()
{
	if (LevelObjects.size() > 0) for (auto& object : LevelObjects) if (object) delete(object);
	if (Meshes.size() > 0) for (auto& mesh : Meshes) if (mesh.second) delete(mesh.second);
	if (Materials.size() > 0) for (auto& material : Materials) if (material.second) delete(material.second);
	if (Textures.size() > 0) for (auto& texture : Textures) if (texture.second) delete(texture.second);
	if (RenderData) delete(RenderData);
}

void O_Level::AddCollider(C_CollisionComponent* collider)
{
	Colliders.push_back(collider);
}

void O_Level::AddLightSource(S_LightInfo* light)
{
	if(RenderData) RenderData->LightSources.insert(light);
}

void O_Level::RemoveLightSource(S_LightInfo* light)
{
	if (RenderData && light) RenderData->LightSources.erase(light);
}
