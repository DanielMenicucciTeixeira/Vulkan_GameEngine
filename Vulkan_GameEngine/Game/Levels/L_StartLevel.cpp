#include "L_StartLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"
#include "GO_Camera.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

L_StartLevel::L_StartLevel() : O_Level()
{
	Name = "StartLevel";
}

void L_StartLevel::Start()
{

	SpawnGameObjectOfClass<GO_Camera>();
	O_Level::Start();
	printf("StartLevel Started!\n");
}

void L_StartLevel::Update(float deltaTime)
{
	O_Level::Update(deltaTime);
}

void L_StartLevel::Render()
{
	O_Level::Render();
}
