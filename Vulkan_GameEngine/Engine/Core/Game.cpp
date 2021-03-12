#include "Game.h"
#include "Level.h"
#include "Clock.h"
#include "SDL/SDLManager.h"
#include "Objects/GameObjects/GameObject.h"
#include "DebugLogger.h"

#include <SDL.h>

#include <iostream>
#include <stdexcept>

Game::Game() : Running(false), Paused(false), GameClock(nullptr), InterfaceManager(nullptr), GameRenderer(nullptr), CurrentLevel(nullptr), NextLevel(nullptr), ShouldStartNewLevel(false), FramesPerSecond(60)
{
	
}

Game::~Game()
{
	CleanUp();
}

bool Game::Initialize(SDLManager* interfaceManager, Renderer* gameRenderer)
{
	if (!interfaceManager)
	{
		DebugLogger::FatalError("Failed to get valid interface manager!", "Core/Game.cpp", __LINE__);
		return false;
	}
	if (!CurrentLevel)
	{
		DebugLogger::FatalError("Failed to get valid level!", "Core/Game.cpp", __LINE__);
		return false;
	}
	InterfaceManager = interfaceManager;
	GameClock = new Clock();
	Running = true;
	GameRenderer = gameRenderer;
	CurrentLevel->Initialize(this);
	//TODO implement render choosing mechanic
	/*
	RenderData = new RenderInitializationData();
	InterfaceManager->SetRenderer(GameRenderer);
	InterfaceManager->Begin();
	dynamic_cast<VulkanManager*>(GameRenderer)->SetMainWindow(InterfaceManager->GetSDLWindowByName(InterfaceManager->GetDefaultWindowName()));
	*/
	return true;
}

void Game::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		auto eventType = event.type;
		int32_t keycode;

		//If the event is key or button realted, set the keycode
		switch (eventType)
		{
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERBUTTONDOWN:
			keycode = event.cbutton.button;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keycode = event.key.keysym.sym;
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			keycode = event.button.button;
			break;
		default:
			keycode = SDLK_UNKNOWN;
			break;
		}
		//Then find the function in the map using the event type and keycode as directions, if it exists, call it.
		if (GameInputFunctions[std::make_pair(eventType, keycode)]) GameInputFunctions[std::make_pair(eventType, keycode)](this, &event);
	}
}

void Game::Update(const float deltaTime)
{
	if(CurrentLevel) CurrentLevel->Update(deltaTime);//Remove the extra check at every frame
}

void Game::Render()
{
	
	if (CurrentLevel) CurrentLevel->Render();
}

void Game::SetGameInputFunction(sdlEventType eventType, sdlKeycode keycode, void(*function)(Game*, SDL_Event*))
{
	//If the event is not key realted, mark the keycode as Unknown
	if (
		eventType != SDL_KEYUP && eventType != SDL_KEYDOWN &&
		eventType != SDL_MOUSEBUTTONUP && eventType != SDL_MOUSEBUTTONDOWN &&
		eventType != SDL_CONTROLLERBUTTONUP && eventType != SDL_CONTROLLERBUTTONDOWN
		)
	{
		keycode = SDLK_UNKNOWN;
	}

	//Then map the function to the EngineInputFunctions map
	GameInputFunctions[std::make_pair(eventType, keycode)] = function;
}

Renderer* Game::GetRenderer()
{
	return GameRenderer;
}

void Game::SetPause(const bool& pause)
{
	Paused = pause;
}

float Game::GetTimeSeconds()
{
	return GameClock->GetTimeSeconds();
}

float Game::GetDeltaTimeSeconds()
{
	return GameClock->GetDeltaTimeSeconds();
}

float Game::GetDeltaTimeMiliseconds()
{
	return GameClock->GetDeltaTimeMiliSecods();
}

float Game::GetSleepTime()
{
	return GameClock->GetSleepTime(FramesPerSecond);
}

void Game::SetCurrentLevel()
{
	if (CurrentLevel) CurrentLevel->CleanUp();
	CurrentLevel = NextLevel;
	Levels[CurrentLevel->GetName()] = CurrentLevel;
	CurrentLevel->Initialize(this);
	CurrentLevel->Start();
	ShouldStartNewLevel = false;
}

void Game::CleanUp()
{
	//InterfaceManager->End();
	if (GameClock) delete(GameClock);
	if (GameRenderer) delete(GameRenderer);
	for (auto& level : Levels) delete(level.second);
}

void Game::StartNewLevel(O_Level* level)
{
	NextLevel = level;
	ShouldStartNewLevel = true;
}

void Game::StartNewLevel(std::string levelName)
{
	if (Levels[levelName])
	{
		NextLevel = Levels[levelName];
		ShouldStartNewLevel = true;
	}
	else DebugLogger::Error("Invalid level name.", "Core/Level.cpp", __LINE__);
}

int Game::Run()
{
	/*
	UniformCameraObject* camera = new UniformCameraObject();
	camera->View.SetToLookAtMatrix(FVector3(0.0f, 0.0f, 8.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
	camera->Projection.SetToPerspectiveMatrix(0.0f, 800.0f / 600.0f, 0.1f, 10.0f);
	RenderData->Camera = camera;
	*/
	GameClock->StartClock();
	try
	{
		CurrentLevel->Start();
		//GameRenderer->Initialize(RenderData);
		while (Running)
		{
			if (ShouldStartNewLevel) SetCurrentLevel();
			GameClock->UpdateClock();
			HandleEvents();
			Update(GameClock->GetDeltaTimeSeconds());
			Render();
			/*
			int w, h;
			SDL_GetWindowSize(InterfaceManager->GetSDLWindowByName(), &w, &h);
			camera->View.SetToLookAtMatrix(FVector3(0.0f, -4.0, 4.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
			camera->Projection.SetToPerspectiveMatrix(60.0f, (float)w / (float)h, 0.1f, 20.0f);
			GameRenderer->Render();
			*/
			SDL_Delay(GameClock->GetSleepTime(FramesPerSecond));
		}
		CleanUp();
		DebugLogger::Info("Game exited successfully!", "Core/Game.cpp", __LINE__);
		//GameRenderer->CleanUp();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//InterfaceManager->End();
}
