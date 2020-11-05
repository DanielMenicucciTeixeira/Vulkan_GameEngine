#include "VGE_SDLEventHandler.h"
#include "Core/EngineEvent.h"

#include <SDL.h>

VGE_SDLEventHandler::VGE_SDLEventHandler()
{
}

VGE_SDLEventHandler::~VGE_SDLEventHandler()
{
}

SDL_Event VGE_SDLEventHandler::HandleEvents()
{
	SDL_Event event;

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0)
    {
        return event;
    }
}


