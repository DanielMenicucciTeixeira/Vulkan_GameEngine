#include "Game01.h"
#include "Levels/L_MainLevel.h"
#include "Levels/L_StartLevel.h"
#include <SDL.h>
#include "Event/EventHandler.h"
#include "CoreEngine.h"

G_Game01::G_Game01() : BaseGame()
{
}

bool G_Game01::Initialize(Renderer* gameRenderer)
{
	/*SetGameInputFunction(SDL_QUIT, SDLK_UNKNOWN, BaseGame::QuitEngine);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_q, BaseGame::QuitGame);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_s, G_Game01::LoadStartLevel);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_m, G_Game01::LoadMainLevel);*/

	EventHandler::AddFunctionByInput(nullptr, QuitApplication, SDL_QUIT);
	EventHandler::AddFunctionByEvent(nullptr, EndGame, "Quit Game");
	
	CurrentLevel = new L_MainLevel();
	return BaseGame::Initialize(gameRenderer);
}

void G_Game01::LoadStartLevel(BaseGame* self, SDL_Event* event)
{
	self->StartNewLevel<L_StartLevel>();
}

void G_Game01::LoadMainLevel(BaseGame* self, SDL_Event* event)
{
	self->StartNewLevel<L_MainLevel>();
}

void G_Game01::EndGame(O_Object* object, SDL_Event* event)
{
	CoreEngine::GetInstance()->GetGame()->SetRunning(false);
}

void G_Game01::QuitApplication(O_Object* object, SDL_Event* event)
{
	CoreEngine::GetInstance()->GetGame()->SetRunning(false);
	CoreEngine::GetInstance()->Quit(event);
}
