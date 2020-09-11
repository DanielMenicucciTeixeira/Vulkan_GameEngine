#ifndef VGE_SDLMANAGER_H
#define VGE_SDLMANAGER_H

class VGE_SDLManager
{
public:
	//-------------Constructors---------------------\\

	VGE_SDLManager();

	//--------------Destructor-----------------------\\

	~VGE_SDLManager();

protected:
	class VGE_Window* WindowManager = nullptr;


	//-----------------------Functions-----------------------\\
	
public:
	void Begin();
	void End();

protected:
	bool InitializeSDL();
};
#endif

