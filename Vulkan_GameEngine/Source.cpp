#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanRenderer.h"
#include "Core/EngineEvent.h"
#include <iostream>

#include <stdexcept>
#include <cstdlib>


int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");

	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	Renderer* GameRenderer = new VulkanRenderer(SDLManager);
	SDLManager->Begin();
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