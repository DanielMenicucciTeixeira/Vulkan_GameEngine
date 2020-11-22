#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanManager.h"
#include "Core/EngineEvent.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	Renderer* GameRenderer = new VulkanManager(SDLManager);
	try 
	{
		GameRenderer->Run();
	}
	catch (const std::exception & e) 
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	SDLManager->End();

	//Cleanup
	if(SDLManager != nullptr) delete(SDLManager);
	if (GameRenderer != nullptr) delete(GameRenderer);
	//-------

	return 0;
}