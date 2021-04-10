#include "Game01.h"
#include "Levels/L_MainLevel.h"
#include "Levels/L_StartLevel.h"
#include <SDL.h>

G_Game01::G_Game01() : Game()
{
}

bool G_Game01::Initialize(Renderer* gameRenderer)
{
	SetGameInputFunction(SDL_QUIT, SDLK_UNKNOWN, Game::QuitEngine);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_q, Game::QuitGame);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_s, G_Game01::LoadStartLevel);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_m, G_Game01::LoadMainLevel);
	CurrentLevel = new L_MainLevel();
	return Game::Initialize(gameRenderer);
}

void G_Game01::LoadStartLevel(Game* self, SDL_Event* event)
{
	self->StartNewLevel<L_StartLevel>();
}

void G_Game01::LoadMainLevel(Game* self, SDL_Event* event)
{
	self->StartNewLevel<L_MainLevel>();
}
