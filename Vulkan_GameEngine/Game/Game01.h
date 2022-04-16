#ifndef GAME01_H
#define GAME01_H

#include "Objects/Object.h"

class G_Game01
{
public:
	G_Game01();
	bool Initialize();
	void Update(float deltaTime_);
	void Render();
	static void LoadLevel(int sceneNum, SDL_Event* event);

	static void EndGame(void* object, SDL_Event* event);
	static void QuitApplication(void* object, SDL_Event* event);
};
#endif

