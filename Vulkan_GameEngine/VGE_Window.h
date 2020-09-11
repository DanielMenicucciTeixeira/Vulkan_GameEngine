#ifndef VGE_WINDOW_H
#define VGE_WINDOW_H

class VGE_Window
{
public:
	//----------------Constructors-------------------\\

	 VGE_Window();

	//---------------Destructor----------------------\\
	
	~VGE_Window();

protected:
	//Screen Dimension Constants
	const unsigned ScreenWidth = 640;
	const unsigned ScreenHeight = 480;

	//The window to be rendered into
	class SDL_Window* EngineWindow = nullptr;

	//Surface is a 2D image, in this case, to be created inside "EngineWindow"
	class SDL_Surface* EngineWindowSurface = nullptr;

	//-----------------------Functions-----------------------\\
	
public:
	bool CreateWindow(const char* windowName, float windowSizeX = -1.0f, float windowSizeY = -1.0f, float windowPositionX = -1.0f, float windowPositionY = -1.0f);

	inline SDL_Window* GetEngineWindow() { return EngineWindow; }

	inline SDL_Surface* GetEngineWindowSurface() { return EngineWindowSurface; }
	inline void SetEngineWindowSurface(SDL_Surface* windowSurface) { EngineWindowSurface = windowSurface; }

};
#endif
