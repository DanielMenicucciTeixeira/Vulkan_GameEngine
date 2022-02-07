#include "Level.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Game.h"
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"
#include "Graphics/AssetLoader.h"
#include "Graphics/TextureLoader.h"
#include "Objects/Components/CameraComponent.h"
#include "Renderers/Renderer.h"
#include "LevelGraph.h"
#include "CollisionHandler.h"

#include <algorithm>

L_Level::L_Level(float worldSize) : CurrentGame(nullptr), NextCamera(nullptr), Name(""), WorldSize(worldSize)
{
	LevelGraph::GetInstance()->GenerateSpationPartition(worldSize);
}

L_Level::~L_Level()
{
}

bool L_Level::Initialize(BaseGame* game)
{
	if (!game) return false;
	LoadModels();
	CurrentGame = game;
	LoadLevelObjects();
	//CurrentGame->GetRenderer()->Initialize();
	return true;
}

void L_Level::Start()
{
	LoadLevelObjects();
	CheckForCamera();
}

void L_Level::LoadModels()
{
	std::set<S_Mesh*> meshSet = std::set<S_Mesh*>();
	for (const auto& path : ModelPaths) AssetLoader::LoadModel(path, meshSet);
	for (const auto& mesh : meshSet) LevelGraph::GetInstance()->AddMesh(mesh);
	ModelPaths.clear();
}
void L_Level::LoadMaterialLibrary()
{
	std::set<S_Material*> materialSet = std::set<S_Material*>();
	for (const auto& path : MaterialPaths) AssetLoader::LoadMaterialLibrary(path, materialSet);
	for (const auto& material : materialSet) LevelGraph::GetInstance()->AddMaterial(material);
	MaterialPaths.clear();
}
void L_Level::LoadMaterial(S_Material* material)
{
	LevelGraph::GetInstance()->AddMaterial(material);
	if (material->TextureNameDifuse != "") LoadTexture(material->TextureDifuse, material->TextureNameDifuse);
	if (material->TextureNameSpecular != "") LoadTexture(material->TextureSpecular, material->TextureNameSpecular);
}

bool L_Level::LoadTexture(S_Texture*& texture, const std::string& textureName)
{
	auto& textures = LevelGraph::GetInstance()->GetTextures();
	if (!textures[textureName])
	{
		DebugLogger::Error("Failed to find texture with name: " + texture->Name + ". ", "Core/Level.cpp", __LINE__);
		return false;
	}

	if (!textures[textureName]->Pixels)
	{
		//TODO: Solve SDL TextureHandler removal here.
		/*
		if (!SDLTextureHandler::LoadTexture(textureName, textures[textureName]->Path, textures[textureName]))
		{
			DebugLogger::Error("Failed to load texture: " + texture->Name + " at " + texture->Path, "Core/Level.cpp", __LINE__);
			return false;
		}
		*/
	}
	
	texture = textures[textureName];
	return true;
}

void L_Level::LoadLevelObjects()
{
	if (UnloadedObjects.empty()) return;

	for (auto& object : UnloadedObjects)
	{
		O_GameObject* gameObject = dynamic_cast<O_GameObject*>(object);
		if (gameObject)
		{
			for (auto& mesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
			{
				mesh->SetMaterial(LevelGraph::GetInstance()->GetMaterials()[mesh->GetMaterialName()]);
				mesh->SetMesh(LevelGraph::GetInstance()->GetMeshes()[mesh->GetMeshName()]);
				LevelGraph::GetInstance()->AddMeshComponent(mesh);
			}
		}
		LevelGraph::GetInstance()->AddObject(object);
		gameObject->Start();
	}
	//TODO: Fix, Also would this not require you to reload every single mesh?
	//CurrentGame->GetRenderer()->UpdateWithNewObjects();
	if (UnloadedObjects.size() > 0)
	{
		UnloadedObjects.clear();
		UnloadedObjects = std::set<O_Object*>();
	}
}

bool L_Level::LoadCamera(C_CameraComponent* camera)
{
	if (!camera)
	{
		DebugLogger::Error("No valid CameraFound!", "Core/Level.cpp", __LINE__);
		return false;
	}
	LevelGraph::GetInstance()->SetActiveCamera(camera);
	return true;
}

bool L_Level::FindAnyCamera()
{
	for (const auto gameObject : LevelGraph::GetInstance()->GetObjects())
	{
		if (dynamic_cast<O_GameObject*>(gameObject.second))
		{
			for (const auto camera : dynamic_cast<O_GameObject*>(gameObject.second)->GetComponentsOfClass<C_CameraComponent>())
			{
				NextCamera = camera;
				return true;
			}
		}
	}
	DebugLogger::Warning("No camera found!", "Core/Level.cpp", __LINE__);
	return false;
}

bool L_Level::ChangeCamera()
{
	if (NextCamera)
	{
		bool returnValue = LoadCamera(NextCamera);
		NextCamera = nullptr;
		return returnValue;
	}
	DebugLogger::Warning("Invalid NextCamera!", "Core/Level.cpp", __LINE__);
	return false;
}

bool L_Level::CheckForCamera()
{
	if (NextCamera)
	{
		if (ChangeCamera()) return true;
		else if (LevelGraph::GetInstance()->ActiveCamera) return true;
		else
		{
			DebugLogger::FatalError("No valid camera found!", "Core/Level.cpp", __LINE__);
			return false;
		}
	}
	else if (!LevelGraph::GetInstance()->ActiveCamera)
	{
		if (FindAnyCamera() && ChangeCamera()) return true;
		else
		{
			DebugLogger::FatalError("No valid camera found!", "Core/Level.cpp", __LINE__);
			return false;
		}
	}

	return true;
}

void L_Level::Update(const float deltaTime)
{
	LoadLevelObjects();
	CheckCollisions();

	auto& levelObjects = LevelGraph::GetInstance()->GetObjects();


	if (!LevelGraph::GetInstance()->GetPaused()) for (const auto& object : levelObjects) object.second->Update(deltaTime);
	else for (const auto& object : levelObjects) if (object.second->UpdateWhenPaused) object.second->Update(deltaTime);

	//check collision here
	CollisionHandler::GetInstance()->Update(deltaTime);
	
}

void L_Level::Render()
{
	LevelGraph::GetInstance()->Render();
	//CurrentGame->GetRenderer()->Render();
}

void L_Level::CleanUp()
{
	LevelGraph::GetInstance()->CleanUp();
}

void L_Level::CheckCollisions()
{
	for (const auto& partition : LevelGraph::ColliderSpationPartition->GetActiveLeaves())
	{
		C_CollisionComponent::CheckForCollisions(partition->GetColliders());
	}
}

void L_Level::AddCollider(C_CollisionComponent* collider)
{
	LevelGraph::GetInstance()->AddCollisionComponent(collider);
}
