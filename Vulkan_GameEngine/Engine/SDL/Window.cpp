#include "Window.h"
#include "SDL/SDLWindowManager.h"
#include "Renderers/Renderer.h"
#include "DebugLogger.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

#include <unordered_map>
#include <string>

Window::Window() : SDLWindow(nullptr), Context(nullptr), WindowManager(nullptr), WindowName("")
{
}

Window::~Window()
{
	WindowManager->DeleteWindow(WindowName);
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(SDLWindow);
	SDLWindow = nullptr;
}

bool Window::Initialize(SDLWindowManager* windowManager, const char* name, ERendererType rendererType, int width, int height, int positionX, int positionY)
{
	if (!windowManager)
	{
		DebugLogger::FatalError("Failed to create Window!", "SLD/Window.cpp", __LINE__);
		return false;
	}
	WindowManager = windowManager;

	WindowName = name;
	
	switch (rendererType)
	{
	case VULKAN:
		return OnCreateVulkanWindow(name, width, height, positionX, positionY);
	case OPEN_GL:
		return OnCreateOpenGLWindow(name, width, height, positionX, positionY);
	}

	DebugLogger::FatalError("Failed to create Window!", "SLD/Window.cpp", __LINE__);
	return false;
}

void Window::SetPreAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
}

void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 64);
}

float Window::GetHeight() const
{
	int w, h;
	SDL_GetWindowSize(SDLWindow, &w, &h);
	return w;
}

float Window::GetWidth() const
{
	int w, h;
	SDL_GetWindowSize(SDLWindow, &w, &h);
	return h;
}

bool Window::OnCreateVulkanWindow(const char* name, int width, int height, int positionX, int positionY)
{
	SDLWindow = SDL_CreateWindow(name, positionX, positionY, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (SDLWindow == NULL)
	{
		DebugLogger::FatalError("Window could not be created! SDL_Error: " + std::string(SDL_GetError()), "SDL/Window.cpp", __LINE__);
		return false;
	}
}

bool Window::OnCreateOpenGLWindow(const char* name, int width, int height, int positionX, int positionY)
{
	SetPreAttributes();
	SDLWindow = SDL_CreateWindow(name, positionX, positionY, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (SDLWindow == NULL)
	{
		DebugLogger::FatalError("Window could not be created! SDL_Error: " + std::string(SDL_GetError()), "SDL/Window.cpp", __LINE__);
		return false;
	}

	Context = SDL_GL_CreateContext(SDLWindow);
	SetPostAttributes();

	if (glewInit() != GLEW_OK)
	{
		DebugLogger::FatalError("Failed to initialize Glew!", "SDL/Window.cpp", __LINE__);
		return false;
	}
	std::string version((const char*)glGetString(GL_VERSION));
	DebugLogger::Info("GL version: " + version, "SDL/Window.cpp", __LINE__);

	glViewport(0, 0, width, height);
	return true;
}
