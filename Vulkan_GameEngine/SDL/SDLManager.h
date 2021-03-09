#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <vector>

enum class E_EngineEvent;
union SDL_Event;
class VGE_VulkanValidationLayers;

class SDLManager
{
public:
	//-------------Constructors---------------------\\

	SDLManager();

	//--------------Destructor-----------------------\\

	~SDLManager();

protected:
	class SDLWindowManager* WindowManager = nullptr;
	class SDLTextureLoader* TextureLoader = nullptr;
	class SDL_Surface* imageSurface = nullptr;
	class SDLEventHandler* EventHandler = nullptr;
	class Renderer* WindowRenderer = nullptr;


	//-----------------------Functions-----------------------\\
	
public:
	void Begin();
	void End();
	SDL_Event GetEvent();
	bool GetVulkanExtensions(std::vector<const char*>& extensionNames);
	
	void SetRenderer(Renderer* renderer);

	class SDL_Window* GetSDLWindowByName(const char* name = GetDefaultWindowName());

	inline static const char* GetDefaultWindowName() { return "window"; }

protected:
	bool InitializeSDL();
};
#endif

