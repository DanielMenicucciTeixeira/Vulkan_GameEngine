#include "SDL/VGE_SDLManager.h"
#include "Vulkan/VulkanManager.h"
#include "Core/EngineEvent.h"
#include "TextureLoader.h"
#include "MeshLoader.h"
#include "RenderObject.h"
#include "UniformBufferObject.h"
#include "RenderInitializationData.h"
#include "GameObject.h"

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

int main(int argc, char* argv[])
{
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");
	
	
	MeshStruct vikingRoomMesh;
	TextureStruct vikingRoomTexture;
	MeshLoader::LoadMesh("Models/viking_room.obj", &vikingRoomMesh);
	TextureLoader::LoadTexture("Textures/viking_room.png", &vikingRoomTexture);

	MeshStruct sphereMesh;
	TextureStruct sphereTexture;
	MeshLoader::LoadMesh("Models/sphere.obj", &sphereMesh);
	TextureLoader::LoadTexture("Textures/texture.jpg", &sphereTexture);


	UniformCameraObject* Camera = new UniformCameraObject();
	

	GameObject* VikingRoom = new GameObject();
	VikingRoom->Mesh = &vikingRoomMesh;
	VikingRoom->Texture = &vikingRoomTexture;

	GameObject* Sphere = new GameObject();
	Sphere->Mesh = &sphereMesh;
	Sphere->Texture = &sphereTexture;

	RenderInitializationData* renderData = new RenderInitializationData();
	renderData->LoadGameObject(VikingRoom);
	renderData->LoadGameObject(Sphere);
	renderData->Camera = Camera;

	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	Renderer* GameRenderer = new VulkanManager(SDLManager->GetWindow());
	SDLManager->SetRenderer(GameRenderer);
	try 
	{
		GameRenderer->Initialize(renderData);
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			UpdateUBO(VikingRoom->Model, Camera);
			Sphere->Model->SetToScalingMatrix(0.2, 0.2, 0.2);

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
	if (Sphere != nullptr) delete(Sphere);
	if (Camera != nullptr) delete(Camera);
	if (renderData != nullptr) delete(renderData);
	//-------

	return 0;
}