#include "Window.h"
#include "SDL/SDLWindowManager.h"
#include "Renderer.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

#include <unordered_map>

Window::Window(SDLWindowManager* windowManager) : SDLWindow(nullptr), Context(nullptr)
{
	WindowManager = windowManager;
}

Window::~Window()
{
	WindowManager->GetWindows().erase(WindowName);
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(SDLWindow);
	SDLWindow = nullptr;
}

bool Window::OnCreate(const char* name, ERendererType rendererType, int width, int height, int positionX, int positionY)
{
	WindowName = name;
	Width = width;
	Height = height;
	
	switch (rendererType)
	{
	case VULKAN:
		return OnCreateVulkanWindow(name, width, height, positionX, positionY);
	case OPEN_GL:
		return OnCreateOpenGLWindow(name, width, height, positionX, positionY);
	}

	printf("Failed to create Window!");
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

bool Window::OnCreateVulkanWindow(const char* name, int width, int height, int positionX, int positionY)
{
	SDLWindow = SDL_CreateWindow(name, positionX, positionY, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (SDLWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
}

bool Window::OnCreateOpenGLWindow(const char* name, int width, int height, int positionX, int positionY)
{
	SetPreAttributes();
	SDLWindow = SDL_CreateWindow(name, positionX, positionY, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GL_CreateContext(SDLWindow);
	if (SDLWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	Context = SDL_GL_CreateContext(SDLWindow);
	SetPostAttributes();

	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize Glew!");
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	printf("GL version: %s.\n", glGetString(GL_VERSION));
	return true;
}
