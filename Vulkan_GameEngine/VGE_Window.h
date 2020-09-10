#ifndef VGE_WINDOW_H
#define VGE_WINDOW_H

class VGE_Window
{
	//Screen Dimension Constants
	const unsigned ScreenWidth = 640;
	const unsigned ScreenHeight = 480;

	//The window to be rendered into
	class SDL_Window* EngineWindow = nullptr;

	//Surface is a 2D image, in this case, to be created inside "EngineWindow"
	class SDL_Surface* EngineWindowSurface = nullptr;

};
#endif
