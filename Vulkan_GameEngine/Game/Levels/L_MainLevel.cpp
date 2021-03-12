#include "L_MainLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
L_MainLevel::L_MainLevel() : O_Level()
{
	Name = "MainLevel";
}

void L_MainLevel::Render()
{
	O_Level::Render();
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(CurrentGame->GetInterfaceManager()->GetSDLWindowByName(CurrentGame->GetInterfaceManager()->GetDefaultWindowName()));
}

void L_MainLevel::Update(float deltaTime)
{
	O_Level::Update(deltaTime);
	printf("MainLevel\tDelta Time: %f\tTotal Time: %f\n", deltaTime, CurrentGame->GetTimeSeconds());
}

void L_MainLevel::Start()
{
	O_Level::Start();
	printf("MainLevel Started!\n");
}
