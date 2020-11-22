#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanRenderer.h"
#include "Core/EngineEvent.h"
#include <iostream>

#include <stdexcept>
#include <cstdlib>
#include "Math/FMatrix4.h"
#include "Math/FMatrix.h"
#include <glm/glm.hpp>

void glPrint(glm::mat4 mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f""\t", mat[i][j]);
		}
		printf("\n");
	}
}

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