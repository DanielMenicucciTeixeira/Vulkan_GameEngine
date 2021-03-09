#include "CoreEngine.h"
#include "Renderers/Renderer.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");
	
	if (!CoreEngine::GetInstance()->OnCreate("Main Window", OPEN_GL))
	{
		printf("Failed to create Engine!");
		return 0;
	}
	CoreEngine::GetInstance()->Run();
	return 0;
}