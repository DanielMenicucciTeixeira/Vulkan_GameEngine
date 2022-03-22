#include "L_StartLevel.h"
#include "Objects/GameObjects/GO_Camera.h"

#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

L_StartLevel::L_StartLevel() : L_Level()
{
	Name = "StartLevel";
}

bool L_StartLevel::Initialize()
{
	return false;
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
