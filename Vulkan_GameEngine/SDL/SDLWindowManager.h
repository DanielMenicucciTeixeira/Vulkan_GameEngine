#ifndef SDLWINDOWMANAGER_H
#define SDLWINDOWMANAGER_H

#include <unordered_map>

class SDLManager;
class Window;
enum ERendererType;

class SDLWindowManager
{
public:
	//----------------Constructors-------------------\\

	 SDLWindowManager(SDLManager* manager);

	//---------------Destructor----------------------\\
	
	~SDLWindowManager();

protected:
	SDLManager* Manager;

	//Screen Dimension Constants
	const unsigned ScreenWidth = 640;
	const unsigned ScreenHeight = 480;

	//Surface is a 2D image, in this case, to be created inside "EngineWindow"
	class SDL_Surface* WindowSurface = nullptr;

	//Map of all windows
	std::unordered_map<const char*, Window*> WindowsByName;
	//-----------------------Functions-----------------------\\
	
public:
	bool CreateWindow(const char* windowName, ERendererType rendererType, float windowSizeX = -1.0f, float windowSizeY = -1.0f, float windowPositionX = -1.0f, float windowPositionY = -1.0f);

	inline Window* GetWindowByName(const char* name) { return WindowsByName[name]; }
	class SDL_Window* GetSDLWindowByName(const char* name);
	inline std::unordered_map<const char*, Window*> GetWindows() const { return WindowsByName; }

	inline SDL_Surface* GetWindowSurface() const { return WindowSurface; }
	inline void SetWindowSurface(SDL_Surface* windowSurface) { WindowSurface = windowSurface; }


};
#endif
