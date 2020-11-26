#ifndef VGE_SDLEVNENTHANDLER_H
#define VGE_SDLEVENTHANDLER_H

union SDL_Event;
class Renderer;

class VGE_SDLEventHandler
{
public:
	//-------------Constructors---------------------\\

	VGE_SDLEventHandler(Renderer* renderer);

	//--------------Destructor-----------------------\\

	~VGE_SDLEventHandler();

	Renderer* WindowRenderer;

protected:

	//----------Fucntions-----------\\

public:

	SDL_Event HandleEvents();//Gets and translats SDL_Events to Engine Enum event, so it can be processed without SDL.

};
#endif

