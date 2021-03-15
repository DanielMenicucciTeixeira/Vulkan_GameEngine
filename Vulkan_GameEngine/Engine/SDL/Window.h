#ifndef WINDOW_H
#define WINDOW_H

#include "Auxiliary/Singleton.h"

class SDLWindowManager;
struct SDL_Window;
typedef void* SDL_GLContext;
enum ERendererType;

class Window : public NoCopyClass<Window>
{
public:
	Window();
	~Window();

	bool Initialize(SDLWindowManager* windowManager, const char* name, ERendererType renderType, int width = -1, int height = -1, int positionX = -1, int positionY = -1);

	float GetWidth() const;
	float GetHeight() const;
	inline const char* GetName() const { return WindowName; }
	inline SDL_Window* GetSDLWindow() const { return SDLWindow; }
	inline SDL_GLContext GetContext() const { return Context; }

protected:

	void SetPreAttributes();
	void SetPostAttributes();

	bool OnCreateVulkanWindow(const char* name, int width = -1, int height = -1, int positionX = -1, int positionY = -1);
	bool OnCreateOpenGLWindow(const char* name, int width = -1, int height = -1, int positionX = -1, int positionY = -1);

	SDLWindowManager* WindowManager;
	const char* WindowName;
	SDL_Window* SDLWindow;
	SDL_GLContext Context;
};
#endif
