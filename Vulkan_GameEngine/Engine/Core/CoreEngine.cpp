#include "CoreEngine.h"
#include "Renderers/Vulkan/VulkanManager.h"
#include "Renderers/OpenGL/OpelGLManager.h"
#include "Clock.h"
#include "DebugLogger.h"
#include "Game.h"
#include "Event/EventListener.h"
#include "Math/FVector2.h"
#include "LevelGraph.h"
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

std::unique_ptr<CoreEngine> CoreEngine::Instance = nullptr;

bool CoreEngine::StartGame()
{
	return true;
}

bool CoreEngine::AddGameEvent(const char* eventName)
{
	
	return false;
}

bool CoreEngine::RemoveGameEvent(const char* eventName)
{
	return false;
}

void CoreEngine::AddInputsToGameEvent(const char* eventName, std::set<SDL_Event> events)
{
}

void CoreEngine::RemoveInputsFromGameEvent(const char* eventName, std::set<SDL_Event> events)
{
}

CoreEngine::CoreEngine() : engineWindow(nullptr), RunningEngine(false), EngineRenderer(nullptr), FramesPerSecond(60), StartingLevel(nullptr), gameInterface(nullptr), currentSceneNumber(0)
{
}

CoreEngine::~CoreEngine()
{
}

bool CoreEngine::Initialize(const char* name, ERendererType renderType, int width, int height, int positionX, int positionY)
{
	DebugLogger::Initialize();

	engineWindow = new Window();

	if (!engineWindow->Initialize(name, renderType))
	{
		CleanUp();
		DebugLogger::FatalError("Window failed to initialize", "CoreEngine", __LINE__);
		return RunningEngine = false;
	}

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

	//Initalize Renderer
	if (EngineRenderer->Initialize()) {
		DebugLogger::FatalError("Renderer could not be initalized", "CoreEngine.cpp", __LINE__);
		return RunningEngine = false;
	}

	EventListener::Initialize();
	
	if (gameInterface) {
		if (!gameInterface->OnCreate()) {
			DebugLogger::FatalError("GameInterface could not be created", "CoreEngine.cpp", __LINE__);
			return RunningEngine = false;
		}
	}



	EngineClock.StartClock();


	EventListener::AddEvent("Quit Game");
	EventListener::AddInputToEvent("Quit Game", SDL_KEYDOWN, SDLK_q);

	DebugLogger::Info("Engine initilized successfully.", "Core/CoreEngine.cpp", __LINE__);
	return RunningEngine = true;
}

void CoreEngine::Run()
{
	EngineClock.StartClock();
	while (RunningEngine)
	{
		EngineClock.UpdateClock();

		HandleEvents();
		Update(EngineClock.GetDeltaTimeSeconds());
		Render(); 
		SDL_Delay(EngineClock.GetSleepTime(FramesPerSecond));
	}
	CleanUp();
}

FVector2 CoreEngine::GetWindowSize()
{
	return FVector2(engineWindow->GetWidth(), engineWindow->GetHeight());
}

//Any way to streamline it? any improvements? honestly I don't know.
void CoreEngine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		auto eventType = event.type;
		int32_t keycode;

		if (/*event.type == SDL_WINDOWEVENT &&*/ event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			EngineRenderer->FramebufferResizeCallback();
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
		}
		//Then find the function in the map using the event type and keycode as directions, if it exists, call it.
		if (EngineInputFunctions[std::make_pair(eventType, keycode)])
		{
			EngineInputFunctions[std::make_pair(eventType, keycode)](&event);
		}
	}
}

void CoreEngine::Update(const float deltaTime)
{
	gameInterface->Update(deltaTime);
}

void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!gameInterface) { gameInterface->Render(); }
	//if (CurrentGame) CurrentGame->Render();//must implement multiple windows to run game and engine simultaneously
	SDL_GL_SwapWindow(engineWindow->GetSDLWindow());
	
	EngineRenderer->Render();
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
	if (EngineRenderer) delete(EngineRenderer);
	exit(0);
}
