#include "VGE_SDLEventHandler.h"
#include "Core/EngineEvent.h"

#include <SDL.h>

VGE_SDLEventHandler::VGE_SDLEventHandler()
{
}

VGE_SDLEventHandler::~VGE_SDLEventHandler()
{
}

E_EngineEvent VGE_SDLEventHandler::HandleEvents()
{
	SDL_Event event;

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0)
    {
        //Translate SDL_Event to E_Engine_Event
        switch (event.type)
        {
        case SDL_QUIT:
            return E_EngineEvent::QUIT;
        default:
            return E_EngineEvent::NONE;
        }
    }
}


