#include "VGE_SDLEventHandler.h"
#include "Core/EngineEvent.h"
#include "Renderer.h"

#include <SDL.h>

VGE_SDLEventHandler::VGE_SDLEventHandler(Renderer* renderer)
{
    WindowRenderer = renderer;
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
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            WindowRenderer->FramebufferResizeCallback();
        }
        return event;
    }
}


