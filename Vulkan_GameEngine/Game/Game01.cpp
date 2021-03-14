#include "Game01.h"
#include "Levels/L_MainLevel.h"
#include "Levels/L_StartLevel.h"
#include <SDL.h>

G_Game01::G_Game01() : Game()
{
}

bool G_Game01::Initialize(SDLManager* interfaceManager, Renderer* gameRenderer)
{
	SetGameInputFunction(SDL_KEYDOWN, SDLK_q, Game::QuitGame);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_s, G_Game01::LoadStartLevel);
	SetGameInputFunction(SDL_KEYDOWN, SDLK_m, G_Game01::LoadMainLevel);
	CurrentLevel = new L_StartLevel();
	return Game::Initialize(interfaceManager, gameRenderer);
}

void G_Game01::LoadStartLevel(Game* self, SDL_Event* event)
{
	self->StartNewLevel<L_StartLevel>();
}

void G_Game01::LoadMainLevel(Game* self, SDL_Event* event)
{
	self->StartNewLevel<L_MainLevel>();
}