#include "Engine/Core/CoreEngine.h"
#include "Game/Game01.h"
#include "Engine/Renderers/Renderer.h"
#include "Engine/Math/FVector3.h"
#include "Engine/Math/FMatrix4.h"
#include "Engine/Geometry/Simplex.h"

#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <set>
#include <glm/glm.hpp>
#include "Game/SceneManager.h"

void printUp(SDL_Event* event) { printf("Key up!\n"); }
void printDown(SDL_Event* event) { printf("Key down!\n"); }
void printEngine(SDL_Event* event) { printf("In Engine!\n"); }


int main(int argc, char* argv[])
{
	std::system("Engine\\Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	CoreEngine::GetInstance()->SetGameInterface(new SceneManager);

	if (!CoreEngine::GetInstance()->Initialize("Window", OPEN_GL))
	{
		printf("Failed to create Engine!");
		return 0;
	}

	//Key bindings

	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_QUIT, SDLK_UNKNOWN, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_q, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_g, CoreEngine::LoadGame);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_r, printDown);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYUP, SDLK_r, printUp);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_t, printEngine);

	//Game start
	CoreEngine::GetInstance()->Run();

	
	return 0;
}