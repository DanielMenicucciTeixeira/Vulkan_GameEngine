#ifndef VGE_SDLMANAGER_H
#define VGE_SDLMANAGER_H

enum class E_EngineEvent;

class VGE_SDLManager
{
public:
	//-------------Constructors---------------------\\

	VGE_SDLManager();

	//--------------Destructor-----------------------\\

	~VGE_SDLManager();

protected:
	class VGE_Window* WindowManager = nullptr;
	class VGE_SDLTextureLoader* TextureLoader = nullptr;
	class SDL_Surface* imageSurface = nullptr;
	class VGE_SDLEventHandler* EventHandler = nullptr;


	//-----------------------Functions-----------------------\\
	
public:
	void Begin();
	void End();
	E_EngineEvent GetEvent();
	void Render();

protected:
	bool InitializeSDL();
};
#endif

