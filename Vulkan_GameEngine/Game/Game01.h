#ifndef GAME01_H
#define GAME01_H

#include "Game.h"

class G_Game01 : public Game
{
public:
	G_Game01();
	virtual bool Initialize(SDLManager* interfaceManager, Renderer* gameRenderer) override;
	static void LoadStartLevel(Game* self, SDL_Event* event);
	static void LoadMainLevel(Game* self, SDL_Event* event);
};
#endif

