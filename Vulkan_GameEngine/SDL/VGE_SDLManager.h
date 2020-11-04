#ifndef VGE_SDLMANAGER_H
#define VGE_SDLMANAGER_H

#include <vector>

enum class E_EngineEvent;
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


	//-----------------------Functions-----------------------\\
	
public:
	void Begin();
	void End();
	E_EngineEvent GetEvent();
	void Render();
	bool GetVulkanExtensions(std::vector<const char*>& extensionNames);

	class SDL_Window* GetWindow();

protected:
	bool InitializeSDL();
};
#endif

