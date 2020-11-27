#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanManager.h"
#include "Core/EngineEvent.h"
#include "TextureLoader.h"
#include "MeshLoader.h"
#include "RenderObject.h"
#include "GameObject.h"
#include "RendererInitializationData.h"

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <chrono>

void UpdateUBO(UniformBufferObject* ubo)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	ubo->Model.SetToRotationMatrix(time * 90.0f, 0.0f, 0.0f, 1.0f);
	ubo->View.SetToLookAtMatrix(FVector3(2.0f, 2.0f, 2.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
	ubo->Projection.SetToPerspectiveMatrix(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);
}

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	GameObject* VikingRoom = new GameObject();
	VikingRoom->Mesh = new MeshStruct();
	VikingRoom->Texture = new TextureStruct();
	MeshLoader::LoadMesh("Models/viking_room.obj", VikingRoom->Mesh);
	TextureLoader::LoadTexture("Textures/viking_room.png", VikingRoom->Texture);
	VikingRoom->UBO = new UniformBufferObject();

	RendererInitializationData renderData;
	renderData.MaterialToUBOMap[VikingRoom->Texture].push_back(VikingRoom->UBO);
	renderData.UBOs.push_back(VikingRoom->UBO);
	renderData.MeshToMaterialMap[VikingRoom->Mesh].push_back(VikingRoom->Texture);

	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	Renderer* GameRenderer = new VulkanManager(SDLManager->GetWindow());
	SDLManager->SetRenderer(GameRenderer);
	try 
	{
		GameRenderer->Initialize(&renderData);
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			UpdateUBO(VikingRoom->UBO);

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
	if (VikingRoom != nullptr) delete(VikingRoom);

	return 0;
}