#include "CoreEngine.h"
#include "Game01.h"
#include "SDL/SDLManager.h"
#include "Renderers/Renderer.h"

#include <SDL.h>
#include <iostream>
#include <cstdlib>

void printUp(SDL_Event* event) { printf("Key up!\n"); }
void printDown(SDL_Event* event) { printf("Key down!\n"); }
void printEngine(SDL_Event* event) { printf("In Engine!\n"); }

int main(int argc, char* argv[])
{

	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	if (!CoreEngine::GetInstance()->Initialize(CoreEngine::GetInstance()->GetInterfaceManager()->GetDefaultWindowName(), OPEN_GL))
	{
		printf("Failed to create Engine!");
		return 0;
	}
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_QUIT, SDLK_UNKNOWN, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_q, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_g, CoreEngine::LoadGame);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_r, printDown);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYUP, SDLK_r, printUp);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_t, printEngine);

	G_Game01* game = new G_Game01();
	CoreEngine::GetInstance()->SetGame(game);

	CoreEngine::GetInstance()->Run();
	return 0;
}