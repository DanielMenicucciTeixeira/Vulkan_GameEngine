#include "L_StartLevel.h"
#include "Game.h"
#include "GO_Camera.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

L_StartLevel::L_StartLevel() : L_Level()
{
	Name = "StartLevel";
}

void L_StartLevel::Start()
{

	SpawnGameObjectOfClass<GO_Camera>();
	L_Level::Start();
	printf("StartLevel Started!\n");
}

void L_StartLevel::Update(float deltaTime)
{
	L_Level::Update(deltaTime);
}

void L_StartLevel::Render()
{
	L_Level::Render();
}
