#include "CoreEngine.h"
#include "Renderers/Renderer.h"

#include <SDL.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");
	
	if (!CoreEngine::GetInstance()->Initialize("Main Window", OPEN_GL))
	{
		printf("Failed to create Engine!");
		return 0;
	}
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_QUIT, SDLK_UNKNOWN, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_r, CoreEngine::LoadGame);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_q, CoreEngine::Quit);
	CoreEngine::GetInstance()->Run();
	return 0;
}