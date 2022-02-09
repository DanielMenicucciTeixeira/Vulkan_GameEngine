#include "Game.h"
#include "Level.h"
#include "Objects/GameObjects/GameObject.h"
#include "LevelGraph.h"
#include "CoreEngine.h"
#include "Event/EventListener.h"

#include <SDL.h>

#include <iostream>
#include <stdexcept>

BaseGame::BaseGame() :  CurrentLevel(nullptr)
{
	
}

BaseGame::~BaseGame()
{
}

bool BaseGame::Initialize()
{
	if (!CurrentLevel)
	{
		DebugLogger::FatalError("Failed to get valid level!", "Core/Game.cpp", __LINE__);
		return false;
	}
	CurrentLevel->Initialize();
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


//TODO: there was a note in update about removing an extra check at every frame


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

void BaseGame::CleanUp()
{
	if (CurrentLevel)
	{
		CurrentLevel->CleanUp();
		delete(CurrentLevel);
		CurrentLevel = nullptr;
	}
}
