#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <memory>
#include <vector>

enum class E_EngineEvent;
union SDL_Event;
class Window;
enum ERendererType;

class SDLManager
{
protected:
	class SDLWindowManager* WindowManager = nullptr;
	class SDLTextureLoader* TextureLoader = nullptr;
	class SDL_Surface* imageSurface = nullptr;
	class SDLEventHandler* EventHandler = nullptr;
	class Renderer* WindowRenderer = nullptr;


	//-----------------------Functions-----------------------\\
	
public:
	static SDLManager* GetInstance();

	SDLManager(const SDLManager&) = delete;
	SDLManager(SDLManager&&) = delete;
	SDLManager& operator=(const SDLManager&) = delete;
	SDLManager& operator=(SDLManager&&) = delete;

	bool Begin();
	void End();
	SDL_Event GetEvent();
	bool GetVulkanExtensions(std::vector<const char*>& extensionNames);
	Window* CreateWindow(const char* windowName, ERendererType rendererType, float windowSizeX = -1.0f, float windowSizeY = -1.0f, float windowPositionX = -1.0f, float windowPositionY = -1.0f);
	void SetRenderer(Renderer* renderer);

	class SDL_Window* GetSDLWindowByName(const char* name = GetDefaultWindowName());
	Window* GetWindowByName(const char* name = GetDefaultWindowName());

	inline static const char* GetDefaultWindowName() { return "window"; }

protected:
	bool InitializeSDL();
	static std::unique_ptr<SDLManager> Instance;
	friend std::default_delete<SDLManager>;

private:
	SDLManager();
	~SDLManager();
};
#endif

