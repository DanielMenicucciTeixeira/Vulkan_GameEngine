#include "VGE_SDLManager.h"
#include "VGE_VulkanManager.h"
#include "HelloTriangle.h"
#include <iostream>

#include <stdexcept>
#include <cstdlib>


int main(int argc, char* argv[])
{
	VGE_SDLManager* SDLManager = new VGE_SDLManager();
    VGE_VulkanManager* VulkanManager = new VGE_VulkanManager();
    

	SDLManager->Begin();

	try {
		VulkanManager->run();
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