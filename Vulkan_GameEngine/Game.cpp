#include "Game.h"
#include "Level.h"
#include "Clock.h"
#include "RenderObject.h"
#include "RenderInitializationData.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "Vulkan/VulkanManager.h"
#include "SDL/VGE_SDLManager.h"
#include "GameObject.h"
#include "SDL.h"

#include <iostream>
#include <stdexcept>

Game::Game()
{
	GameClock = new Clock();
	RenderData = new RenderInitializationData();
	SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	GameRenderer = new VulkanManager(SDLManager->GetWindow());
	SDLManager->SetRenderer(GameRenderer);
}

Game::~Game()
{
	SDLManager->End();
	if (GameClock) delete(GameClock);
	if (RenderData) delete(RenderData);
	if (GameRenderer) delete(GameRenderer);

	for (auto& level : Levels) delete(level.second);
	for (auto& mesh : Meshes) delete(mesh.second);
	for (auto& material : Materials) delete(material.second);
}

void Game::Start()
{
	CurrentLevel->Start();
}

void Game::HandleEvents()
{
}

void Game::Update()
{
	CurrentLevel->Update(GameClock->GetDeltaTimeSeconds());
}

void Game::SetPause(const bool& pause)
{
	Paused = pause;
}

void Game::LoadLevel(O_Level* level)
{
	CurrentLevel = level;
	Levels[level->GetName()] = level;
}

void Game::LoadLevel(std::string levelName)
{
	if (Levels[levelName]) CurrentLevel = Levels[levelName];
	else printf("Invalid level name.");
}

void Game::LoadMesh(S_Mesh* mesh)
{
	if (!Meshes[mesh->Name]) Meshes[mesh->Name] = mesh;
	MeshLoader::LoadMesh(mesh->Path, mesh);
}

void Game::LoadMaterial(S_Material* material)
{
	if (!Materials[material->Name]) Materials[material->Name] = material;
	TextureLoader::LoadTexture(material->TextureDifuse->Path, material->TextureDifuse);
}

void Game::LoadLevelObjects(std::set<O_Object*> levelObjects)
{
	for (auto& object : levelObjects)
	{
		O_GameObject* gameObject = dynamic_cast<O_GameObject*>(object);
		if (gameObject)
		{
			for (auto& mesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
			{
				mesh->SetTexture(Materials[mesh->GetTextureName()]->TextureDifuse);
				mesh->SetMesh(Meshes[mesh->GetMeshName()]);
			}
			RenderData->LoadGameObject(dynamic_cast<O_GameObject*>(object));
		}
	}

}

int Game::Run()
{
	CurrentLevel;
	UniformCameraObject* camera = new UniformCameraObject();
	camera->View.SetToLookAtMatrix(FVector3(0.0f, 0.0f, 8.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
	camera->Projection.SetToPerspectiveMatrix(0.0f, 800.0f / 600.0f, 0.1f, 10.0f);
	RenderData->Camera = camera;
	try
	{
		Start();
		GameRenderer->Initialize(RenderData);
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			HandleEvents();
			Update();
			int w, h;
			SDL_GetWindowSize(SDLManager->GetWindow(), &w, &h);
			camera->View.SetToLookAtMatrix(FVector3(0.0f, -4.0, 4.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
			camera->Projection.SetToPerspectiveMatrix(60.0f, (float)w / (float)h, 0.1f, 20.0f);
			GameRenderer->Render();
		}
		GameRenderer->CleanUp();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	SDLManager->End();
}

S_Mesh* Game::GetMesh(std::string meshName)
{

	return Meshes[meshName];
}

S_Material* Game::GetMaterial(std::string materialName)
{
	return Materials[materialName];
}
