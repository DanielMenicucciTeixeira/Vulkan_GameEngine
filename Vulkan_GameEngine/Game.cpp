#include "Game.h"
#include "Level.h"
#include "Clock.h"
#include "RenderObject.h"
#include "RenderInitializationData.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "Vulkan/VulkanManager.h"
#include "SDL/VGE_SDLManager.h"

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

int Game::Run()
{
	try
	{
		GameRenderer->Initialize(RenderData);
		Start();
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			HandleEvents();
			Update();
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
	delete(this);
}

S_Mesh* Game::GetMesh(std::string meshName)
{

	return Meshes[meshName];
}

S_Material* Game::GetMaterial(std::string materialName)
{
	return Materials[materialName];
}
