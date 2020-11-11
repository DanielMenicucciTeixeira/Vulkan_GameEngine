#ifndef VGE_SDLMANAGER_H
#define VGE_SDLMANAGER_H

#include <vector>

enum class E_EngineEvent;
union SDL_Event;
class VGE_VulkanValidationLayers;

class VGE_SDLManager
{
public:
	//-------------Constructors---------------------\\

	VGE_SDLManager();

	//--------------Destructor-----------------------\\

	~VGE_SDLManager();

protected:
	class VGE_SDLWindowManager* WindowManager = nullptr;
	class VGE_SDLTextureLoader* TextureLoader = nullptr;
	class SDL_Surface* imageSurface = nullptr;
	class VGE_SDLEventHandler* EventHandler = nullptr;
	class Renderer* WindowRenderer = nullptr;


	//-----------------------Functions-----------------------\\
	
public:
	void Begin();
	void End();
	SDL_Event GetEvent();
	void Render();
	bool GetVulkanExtensions(std::vector<const char*>& extensionNames);
	
	inline void SetRenderer(Renderer* renderer) { WindowRenderer = renderer; }

	class SDL_Window* GetWindow();

protected:
	bool InitializeSDL();
};
#endif

