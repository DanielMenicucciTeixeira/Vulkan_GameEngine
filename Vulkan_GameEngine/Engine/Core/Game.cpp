#include "Game.h"
#include "Level.h"
#include "Clock.h"
#include "SDL/SDLManager.h"
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/Renderer.h"
#include "LevelGraph.h"
#include "CoreEngine.h"
#include "Event/EventHandler.h"

#include <SDL.h>

#include <iostream>
#include <stdexcept>

BaseGame::BaseGame() : Running(false), Paused(false), GameClock(nullptr), GameRenderer(nullptr), CurrentLevel(nullptr), NextLevel(nullptr), ShouldStartNewLevel(false), FramesPerSecond(60)
{
	
}

BaseGame::~BaseGame()
{
}

bool BaseGame::Initialize(Renderer* gameRenderer)
{
	if (!CurrentLevel)
	{
		DebugLogger::FatalError("Failed to get valid level!", "Core/Game.cpp", __LINE__);
		return false;
	}
	GameClock = new Clock();
	Running = true;
	GameRenderer = gameRenderer;
	CurrentLevel->Initialize(this);
	EventHandler::SetGameReference(this);
	//TODO implement render choosing mechanic
	/*
	RenderData = new RenderInitializationData();
	InterfaceManager->SetRenderer(GameRenderer);
	InterfaceManager->Begin();
	dynamic_cast<VulkanManager*>(GameRenderer)->SetMainWindow(InterfaceManager->GetSDLWindowByName(InterfaceManager->GetDefaultWindowName()));
	*/
	return true;
}

void BaseGame::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		auto eventType = event.type;
		int32_t keycode;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			GameRenderer->FramebufferResizeCallback();
			LevelGraph::GetInstance()->FrameBufferResizeCallback();
		}

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

void BaseGame::Update(const float deltaTime)
{
	if(CurrentLevel) CurrentLevel->Update(deltaTime);//Remove the extra check at every frame
}

void BaseGame::Render()
{
	if (CurrentLevel) CurrentLevel->Render();
}

void BaseGame::SetGameInputFunction(sdlEventType eventType, sdlKeycode keycode, void(*function)(BaseGame*, SDL_Event*))
{
	//If the event is not key realted, mark the keycode as Unknown
	if (!(
		eventType == SDL_KEYUP || eventType == SDL_KEYDOWN ||
		eventType == SDL_MOUSEBUTTONUP || eventType == SDL_MOUSEBUTTONDOWN ||
		eventType == SDL_CONTROLLERBUTTONUP || eventType == SDL_CONTROLLERBUTTONDOWN
		))
	{
		keycode = SDLK_UNKNOWN;
	}

	//Then map the function to the EngineInputFunctions map
	GameInputFunctions[std::make_pair(eventType, keycode)] = function;
}

void BaseGame::QuitEngine(BaseGame* self, SDL_Event* event)
{
	self->SetRunning(false);
	CoreEngine::GetInstance()->Quit(event);
}

Renderer* BaseGame::GetRenderer()
{
	return GameRenderer;
}

void BaseGame::SetPause(const bool& pause)
{
	Paused = pause;
}

inline SDLManager* BaseGame::GetInterfaceManager()
{
	return SDLManager::GetInstance();
}

float BaseGame::GetTimeSeconds()
{
	return GameClock->GetTimeSeconds();
}

float BaseGame::GetDeltaTimeSeconds()
{
	return GameClock->GetDeltaTimeSeconds();
}

float BaseGame::GetDeltaTimeMiliseconds()
{
	return GameClock->GetDeltaTimeMiliSecods();
}

float BaseGame::GetSleepTime()
{
	return GameClock->GetSleepTime(FramesPerSecond);
}

void BaseGame::SetCurrentLevel()
{
	if (CurrentLevel)
	{
		CurrentLevel->CleanUp();
		delete(CurrentLevel);
		CurrentLevel = nullptr;
	}
	CurrentLevel = NextLevel;
	if (!CurrentLevel->Initialize(this))
	{
		DebugLogger::FatalError("Failed to initialize level!", "Core/Game.cpp", __LINE__);
		SetRunning(false);
		return;
	}
	CurrentLevel->Start();
	ShouldStartNewLevel = false;
}

void BaseGame::CleanUp()
{
	//InterfaceManager->End();
	if (GameClock) delete(GameClock);
	if (CurrentLevel)
	{
		CurrentLevel->CleanUp();
		delete(CurrentLevel);
		CurrentLevel = nullptr;
	}
	NextLevel = nullptr;
}

int BaseGame::Run()
{
	GameClock->StartClock();
	
	try
	{
		CurrentLevel->Start();
		EventHandler::Initialize();
		while (Running)
		{
			if (ShouldStartNewLevel)
			{
				SetCurrentLevel();
			}
			if (!CurrentLevel->CheckForCamera()) break;
			GameClock->UpdateClock();
			EventHandler::HandleEvents();
			Update(GameClock->GetDeltaTimeSeconds());
			Render();
			SDL_Delay(GameClock->GetSleepTime(FramesPerSecond));
		}
		CleanUp();
		DebugLogger::Info("Game exited successfully!", "Core/Game.cpp", __LINE__);
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
