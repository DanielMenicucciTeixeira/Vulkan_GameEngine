#include "CoreEngine.h"
#include "SDL/SDLManager.h"
#include "SDL/SDLWindowManager.h"
#include "SDL/Window.h"
#include "Renderers/Vulkan/VulkanManager.h"
#include "Clock.h"
#include "DebugLogger.h"
#include "GameInterface.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

std::unique_ptr<CoreEngine> CoreEngine::Instance = nullptr;

bool CoreEngine::StartGame()
{
	if (!Game || !Game->Initialize(this))
	{
		DebugLogger::Warning("Failed to load game!", "Core/CoreEngine.cpp", __LINE__);
		return  RunningGame = false;
	}
	return RunningGame = true;
}

CoreEngine::CoreEngine() : EngineWindow(nullptr), RunningEngine(false), RunningGame(false), InterfaceManager(nullptr), EngineRenderer(nullptr), EngineClock(nullptr), FramesPerSecond(60), Game(nullptr)
{
}

CoreEngine::~CoreEngine()
{
}

bool CoreEngine::Initialize(const char* name, ERendererType renderType, int width, int height, int positionX, int positionY)
{
	DebugLogger::Initialize();

	InterfaceManager = new SDLManager();
	if (!InterfaceManager->Begin())
	{
		CleanUp();
		return RunningEngine = false;
	}

	EngineWindow = InterfaceManager->CreateWindow(name, renderType, width, height, positionX, positionY);
	if (!EngineWindow)
	{
		CleanUp();
		return RunningEngine = false;
	}
	DebugLogger::Info("Engine initilized successfully.", "Core/CoreEngine.cpp", __LINE__);

	EngineClock = new Clock();
	EngineClock->StartClock();

	return RunningEngine = true;
}

void CoreEngine::Run()
{
	while (RunningEngine)
	{
		if (Game && RunningGame) Game->Run();

		EngineClock->UpdateClock();
		HandleEvents();
		Update(EngineClock->GetDeltaTimeSeconds());
		Render();
		SDL_Delay(EngineClock->GetSleepTime(FramesPerSecond));
	}
	CleanUp();
}

void CoreEngine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		auto eventType = event.type;
		int32_t keycode;

		//If the event is key or button realted, set the keycode
		switch (eventType)
		{
		case SDL_CONTROLLERBUTTONUP | SDL_CONTROLLERBUTTONDOWN:
			keycode = event.cbutton.button;
			break;
		case SDL_KEYUP | SDL_KEYDOWN:
			keycode = event.key.keysym.sym;
			break;
		case SDL_MOUSEBUTTONUP | SDL_MOUSEBUTTONDOWN:
			keycode = event.button.button;
			break;
		default:
			keycode = SDLK_UNKNOWN;
		}
		//Then find the function in the map using the event type and keycode as directions, if it exists, call it.
		if (EngineInputFunctions[std::make_pair(eventType, keycode)]) EngineInputFunctions[std::make_pair(eventType, keycode)](this);
	}
}

void CoreEngine::Update(const float deltaTime)
{
	//printf("Delta Time: %f\tTotal Time: %f\n", deltaTime, EngineClock->GetTimeSeconds());
}

void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO Call Game renderer
	SDL_GL_SwapWindow(EngineWindow->GetSDLWindow());
}

void CoreEngine::SetEngineInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(CoreEngine*))
{
	//If the event is not key realted, mark the keycode as Unknown
	if (eventType != SDL_KEYUP && eventType != SDL_KEYDOWN) keycode = SDLK_UNKNOWN;

	//Then map the function to the EngineInputFunctions map
	EngineInputFunctions[std::make_pair(eventType, keycode)] = function;
}

void CoreEngine::CleanUp()
{
	InterfaceManager->End();
	if(EngineClock) delete(EngineClock);
	if (Game) delete(Game);
	exit(0);
}
