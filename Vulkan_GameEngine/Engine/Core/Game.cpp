#include "Game.h"
#include "Level.h"
#include "Clock.h"
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/Renderer.h"
#include "LevelGraph.h"
#include "CoreEngine.h"
#include "Event/EventHandler.h"

#include <SDL.h>

#include <iostream>
#include <stdexcept>

BaseGame::BaseGame() :  CurrentLevel(nullptr), NextLevel(nullptr), ShouldStartNewLevel(false)
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
	CurrentLevel->Initialize(this);
	EventListener::SetGameReference(this);
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
	CoreEngine::GetInstance()->Quit(event);
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
		return;
	}
	CurrentLevel->Start();
	ShouldStartNewLevel = false;
}

void BaseGame::CleanUp()
{
	//InterfaceManager->End();
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
	try
	{
		CurrentLevel->Start();
		EventListener::Initialize();

		int placeholder = 0;
		//TODO:Placeholder
		while (placeholder != 1)
		{
			if (ShouldStartNewLevel)
			{
				SetCurrentLevel();
			}
			if (!CurrentLevel->CheckForCamera()) break;
			EventListener::HandleEvents();
			Render();
		}
		CleanUp();
		DebugLogger::Info("Game exited successfully!", "Core/Game.cpp", __LINE__);
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
