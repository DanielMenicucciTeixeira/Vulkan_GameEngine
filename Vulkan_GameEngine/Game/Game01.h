#ifndef GAME01_H
#define GAME01_H

#include "../Engine/Core/Game.h"

class G_Game01 : public BaseGame
{
public:
	G_Game01();
	virtual bool Initialize(Renderer* gameRenderer) override;
	virtual void Update(float deltaTime_);
	virtual void Render();
	static void LoadStartLevel(BaseGame* self, SDL_Event* event);
	static void LoadMainLevel(BaseGame* self, SDL_Event* event);

	static void EndGame(O_Object* object, SDL_Event* event);
	static void QuitApplication(O_Object* object, SDL_Event* event);
};
#endif

