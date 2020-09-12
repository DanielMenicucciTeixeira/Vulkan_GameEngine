#ifndef VGE_SDLEVNENTHANDLER_H
#define VGE_SDLEVENTHANDLER_H

enum class E_EngineEvent;

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

	E_EngineEvent HandleEvents();//Gets and translats SDL_Events to Engine Enum event, so it can be processed without SDL.

};
#endif

