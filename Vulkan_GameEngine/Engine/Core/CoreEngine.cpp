#include "CoreEngine.h"
#include "SDL/SDLManager.h"
#include "SDL/SDLWindowManager.h"
#include "SDL/Window.h"
#include "Renderers/Vulkan/VulkanManager.h"
#include "Renderers/OpenGL/OpelGLManager.h"
#include "Clock.h"
#include "DebugLogger.h"
#include "Game.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

std::unique_ptr<CoreEngine> CoreEngine::Instance = nullptr;

bool CoreEngine::StartGame()
{
	if (!CurrentGame || !CurrentGame->Initialize(SDLManager::GetInstance(), EngineRenderer))//TODO pass a proper renderer!
	{
		DebugLogger::Error("Failed to load game!", "Core/CoreEngine.cpp", __LINE__);
		return  RunningGame = false;
	}
	return RunningGame = true;
}

CoreEngine::CoreEngine() : EngineWindow(nullptr), RunningEngine(false), RunningGame(false), InterfaceManager(nullptr), EngineRenderer(nullptr), EngineClock(nullptr), FramesPerSecond(60), CurrentGame(nullptr), StartingLevel(nullptr)
{
}

CoreEngine::~CoreEngine()
{
}

bool CoreEngine::Initialize(const char* name, ERendererType renderType, int width, int height, int positionX, int positionY)
{
	DebugLogger::Initialize();

	InterfaceManager = SDLManager::GetInstance();
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

	switch (renderType)
	{
	case OPEN_GL:
		EngineRenderer = new OpenGLManager();
		break;
	case VULKAN:
		EngineRenderer = new VulkanManager();
		break;
	default:
		EngineRenderer = new OpenGLManager();
	}

	return RunningEngine = true;
}

void CoreEngine::Run()
{
	EngineClock->StartClock();
	while (RunningEngine)
	{
		if (CurrentGame && CurrentGame->IsRunning()) CurrentGame->Run();// TODO implement multiple windows to run game and engine simultaneously
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
		}
		//Then find the function in the map using the event type and keycode as directions, if it exists, call it.
		if (EngineInputFunctions[std::make_pair(eventType, keycode)]) EngineInputFunctions[std::make_pair(eventType, keycode)](&event);
	}
}

void CoreEngine::Update(const float deltaTime)
{
}

void CoreEngine::Render()
{
	/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//if (CurrentGame) CurrentGame->Render();//must implement multiple windows to run game and engine simultaneously
	SDL_GL_SwapWindow(EngineWindow->GetSDLWindow());*/
}

CoreEngine* CoreEngine::GetInstance()
{
	if (Instance.get() == nullptr) Instance.reset(new CoreEngine);
	return Instance.get();
}

void CoreEngine::SetEngineInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(SDL_Event*))
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
	EngineInputFunctions[std::make_pair(eventType, keycode)] = function;
}

void CoreEngine::CleanUp()
{
	if (EngineClock) delete(EngineClock);
	if (EngineRenderer) delete(EngineRenderer);
	InterfaceManager->End();
	exit(0);
}
