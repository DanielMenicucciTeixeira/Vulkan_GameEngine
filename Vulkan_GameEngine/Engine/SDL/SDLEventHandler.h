#ifndef SDLEVNENTHANDLER_H
#define SDLEVENTHANDLER_H

union SDL_Event;
class Renderer;

class SDLEventHandler
{
public:
	//-------------Constructors---------------------\\

	SDLEventHandler(Renderer* renderer);

	//--------------Destructor-----------------------\\

	~SDLEventHandler();

	Renderer* WindowRenderer;

protected:

	//----------Fucntions-----------\\

public:

	SDL_Event HandleEvents();//Gets and translats SDL_Events to Engine Enum event, so it can be processed without SDL.

};
#endif

