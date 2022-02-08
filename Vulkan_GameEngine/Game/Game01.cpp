#include "Game01.h"
#include "Levels/L_MainLevel.h"
#include "Levels/L_StartLevel.h"
#include "L_TetrahedronLevel.h"
#include <SDL.h>
#include "Event/EventListener.h"
#include "CoreEngine.h"
#include <set>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>

G_Game01::G_Game01()
{
}

bool G_Game01::Initialize()
{
	/*SetGameInputFunction(SDL_QUIT, SDLK_UNKNOWN, BaseGame::QuitEngine);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_q, BaseGame::QuitGame);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_s, G_Game01::LoadStartLevel);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_m, G_Game01::LoadMainLevel);*/

	EventListener::AddFunctionByInput(nullptr, QuitApplication, SDL_QUIT);
	EventListener::AddFunctionByEvent(nullptr, EndGame, "Quit Game");
	
	//CurrentLevel = new L_MainLevel();
	return true;
}

void G_Game01::Update(float deltaTime_)
{
}

void G_Game01::Render()
{
}

void G_Game01::LoadLevel(int sceneNum_, SDL_Event* event)
{
	CoreEngine::GetInstance()->SetCurrentScene(sceneNum_);
}

void G_Game01::EndGame(O_Object* object, SDL_Event* event)
{
	CoreEngine::GetInstance()->Exit();
}

void G_Game01::QuitApplication(O_Object* object, SDL_Event* event)
{
	CoreEngine::GetInstance()->Exit();
	CoreEngine::GetInstance()->Quit(event);
}
