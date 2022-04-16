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

#include "Engine/Serialize/Serialize.h"


void printUp(SDL_Event* event) { printf("Key up!\n"); }
void printDown(SDL_Event* event) { printf("Key down!\n"); }
void printEngine(SDL_Event* event) { printf("In Engine!\n"); }

int main(int argc, char* argv[])
{

	Serialize myObj(10);
	Serialize other(1);

	cout << myObj;
	ofstream outFile("serialized.txt");
	outFile << myObj;
	outFile.close();
	ifstream inFile("serialized.txt");
	inFile >> other;
	inFile.close();
	cout << other;

	


	std::system("Engine\\Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	CoreEngine::GetInstance()->SetGameInterface(new SceneManager);

	if (!CoreEngine::GetInstance()->Initialize("Window", VULKAN))
	{
		printf("Failed to create Engine!");
		return 0;
	}

	//Key bindings

	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_QUIT, SDLK_UNKNOWN, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_q, CoreEngine::Quit);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_r, printDown);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYUP, SDLK_r, printUp);
	CoreEngine::GetInstance()->SetEngineInputFunction(SDL_KEYDOWN, SDLK_t, printEngine);

	//Game start
	CoreEngine::GetInstance()->Run();


	return 0;
}