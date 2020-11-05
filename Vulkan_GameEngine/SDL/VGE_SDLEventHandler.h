#ifndef VGE_SDLEVNENTHANDLER_H
#define VGE_SDLEVENTHANDLER_H

union SDL_Event;

class VGE_SDLEventHandler
{
public:
	//-------------Constructors---------------------\\

	VGE_SDLEventHandler();

	//--------------Destructor-----------------------\\

	~VGE_SDLEventHandler();;

protected:


	//----------Fucntions-----------\\

public:

	SDL_Event HandleEvents();//Gets and translats SDL_Events to Engine Enum event, so it can be processed without SDL.

};
#endif

