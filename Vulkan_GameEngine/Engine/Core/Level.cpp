#include "Level.h"
#include "Objects/Components/CollisionComponent.h"
#include "Game.h"
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "Renderers/Vulkan/VulkanManager.h"
#include "Objects/Components/CameraComponent.h"

O_Level::O_Level() : O_Object(), CurrentGame(nullptr), RenderData(nullptr), CurrentCamera(nullptr)
{
}

O_Level::~O_Level()
{
}

bool O_Level::Initialize(Game* game)
{
	if (!game) return false;
	CurrentGame = game;
	RenderData = new RenderInitializationData();
	LoadLevelObjects();
	CurrentGame->GetRenderer()->Initialize(RenderData);
	return true;
}

void O_Level::Start()
{
	LoadLevelObjects();
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
	TextureLoader::LoadTexture(material->TextureDifuse->Path, material->TextureDifuse);
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
	LoadLevelObjects();
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
	if (RenderData) delete(RenderData);
	if (LevelObjects.size() > 0) for (auto& object : LevelObjects) if (object) delete(object);
	if (Meshes.size() > 0) for (auto& mesh : Meshes) if (mesh.second) delete(mesh.second);
	if (Materials.size() > 0) for (auto& material : Materials) if (material.second) delete(material.second);
}

void O_Level::AddCollider(C_CollisionComponent* collider)
{
	Colliders.push_back(collider);
}
