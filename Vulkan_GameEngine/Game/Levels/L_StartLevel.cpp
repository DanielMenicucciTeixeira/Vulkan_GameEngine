#include "L_StartLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

L_StartLevel::L_StartLevel() : O_Level()
{
	Name = "StartLevel";
}

void L_StartLevel::Start()
{
	O_Level::Start();
	printf("StartLevel Started!\n");
}

void L_StartLevel::Update(float deltaTime)
{
	O_Level::Update(deltaTime);
	printf("StartLevel\tDelta Time: %f\tTotal Time: %f\n", deltaTime, CurrentGame->GetTimeSeconds());
}

void L_StartLevel::Render()
{
	O_Level::Render();
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(CurrentGame->GetInterfaceManager()->GetSDLWindowByName(CurrentGame->GetInterfaceManager()->GetDefaultWindowName()));
}
