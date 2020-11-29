#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanManager.h"
#include "Core/EngineEvent.h"
#include "TextureLoader.h"
#include "MeshLoader.h"
#include "RenderObject.h"
#include "UniformBufferObject.h"
#include "RenderInitializationData.h"
#include "GameObject.h"
#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Math/FQuaternion.h"

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <chrono>

void UpdateUBO(FMatrix4* model, UniformCameraObject* camera)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	model->SetToRotationMatrix(time * 90.0f, 0.0f, 0.0f, 1.0f);
	camera->View.SetToLookAtMatrix(FVector3(2.0f, 2.0f, 2.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
	camera->Projection.SetToPerspectiveMatrix(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);
}

void glPrintMat4(glm::mat4 matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void glPrintQuat(glm::quat quat)
{
	printf("[ %f,\t%f,\t%f,\t%f ]\n", quat.x, quat.y, quat.z, quat.w);
}

int main(int argc, char* argv[])
{

	O_GameObject* Thing = new O_GameObject();
	Thing->AddComponentOfClass<O_Component>();
	delete(Thing);

	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	RenderInitializationData* renderData = new RenderInitializationData();
	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	Renderer* GameRenderer = new VulkanManager(SDLManager->GetWindow());
	SDLManager->SetRenderer(GameRenderer);
	try 
	{
		GameRenderer->Initialize(renderData);
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			GameRenderer->Render();
		}
		GameRenderer->CleanUp();
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
	if (renderData != nullptr) delete(renderData);
	//-------

	return 0;
}