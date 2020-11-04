#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VGE_VulkanManager.h"
#include "HelloTriangle.h"
#include "Core/EngineEvent.h"
#include <iostream>

#include <stdexcept>
#include <cstdlib>


int main(int argc, char* argv[])
{
	VGE_SDLManager* SDLManager = new VGE_SDLManager();
    VGE_VulkanManager* VulkanManager = new VGE_VulkanManager(SDLManager);
    

	SDLManager->Begin();

	try {
		VulkanManager->run();

		while (SDLManager->GetEvent() != E_EngineEvent::QUIT)
		{
			SDLManager->Render();
		}
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	SDLManager->End();

	//Cleanup
	if(SDLManager != nullptr) delete(SDLManager);
	if (VulkanManager != nullptr) delete(VulkanManager);
	//-------

	return 0;
}