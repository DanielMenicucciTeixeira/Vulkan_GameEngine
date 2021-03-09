#include "SDLEventHandler.h"
#include "Core/EngineEvent.h"
#include "Renderers/Renderer.h"

#include <SDL.h>

SDLEventHandler::SDLEventHandler(Renderer* renderer)
{
    WindowRenderer = renderer;
}

SDLEventHandler::~SDLEventHandler()
{
}

SDL_Event SDLEventHandler::HandleEvents()
{
	SDL_Event event;

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            WindowRenderer->FramebufferResizeCallback();
        }
        return event;
    }
}


