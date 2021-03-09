#include "CoreEngine.h"
#include "SDL/SDLManager.h"
#include "SDL/SDLWindowManager.h"
#include "SDL/Window.h"
#include "Renderers/Vulkan/VulkanManager.h"
#include "Clock.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

std::unique_ptr<CoreEngine> CoreEngine::Instance = nullptr;

CoreEngine::CoreEngine() : EngineWindow(nullptr), Running(false), InterfaceManager(nullptr), EngineRenderer(nullptr), EngineClock(nullptr)
{
}

CoreEngine::~CoreEngine()
{
}

CoreEngine* CoreEngine::GetInstance()
{
	if (Instance.get() == nullptr) Instance.reset(new CoreEngine);
	return Instance.get();
}

bool CoreEngine::OnCreate(const char* name, ERendererType renderType, int width, int height, int positionX, int positionY)
{
	EngineClock = new Clock();
	InterfaceManager = new SDLManager();
	if (!InterfaceManager->Begin())
	{
		OnDestroy();
		return Running = false;
	}
	EngineWindow = InterfaceManager->CreateWindow(name, renderType, width, height, positionX, positionY);
	if (!EngineWindow)
	{
		OnDestroy();
		return Running = false;
	}
	return Running = true;
}

void CoreEngine::Run()
{
	while (Running)
	{
		Update(0.0167f);
		Render();
	}
	OnDestroy();
}

void CoreEngine::HandleEvents()
{
}

void CoreEngine::Update(const float deltaTime)
{
}

void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO Call Game renderer
	SDL_GL_SwapWindow(EngineWindow->GetSDLWindow());
}

void CoreEngine::OnDestroy()
{
	InterfaceManager->End();
	exit(0);
}
