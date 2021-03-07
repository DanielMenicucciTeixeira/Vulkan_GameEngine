#include "BilliardGame.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");
	
	
	
	Game* NewGame = new BilliardGame();
	
	NewGame->Run();
	delete(NewGame);
	return 0;
}