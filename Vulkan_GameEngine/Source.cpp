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
#include "StaticMeshComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsLib.h"
#include "Clock.h"
#include "BilliardBall.h"
#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "BilliardTable.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>
#define _USE_MATH_DEFINES
#include "Math/FQuaternion.h"

#include <SDL.h>
#include <SDL_vulkan.h>
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
	std::system("Shaders\\compile.bat");//Compile the shaders to .spv files
	printf("------------------------------------------------------------------------------------------\n\n");

	S_Mesh* sphere = new S_Mesh();
	MeshLoader::LoadMesh("Models/sphere.obj", sphere);
	
	S_Mesh* vikingRoom = new S_Mesh();
	MeshLoader::LoadMesh("Models/viking_room.obj", vikingRoom);

	S_Texture* texture = new S_Texture();
	TextureLoader::LoadTexture("Textures/texture.jpg", texture);

	S_Texture* planet = new S_Texture();
	TextureLoader::LoadTexture("Textures/planet.png", planet);

	S_Texture* vikingTexture = new S_Texture();
	TextureLoader::LoadTexture("Textures/viking_room.png", vikingTexture);

	/*O_GameObject* VikingRoom = new O_GameObject();
	VikingRoom->AddComponentOfClass<C_StaticMeshComponent>();
	for (auto& mesh : VikingRoom->GetComponentsOfClass<C_StaticMeshComponent>())
	{
		mesh->SetMesh(vikingRoom);
		mesh->SetTexture(vikingTexture);
	}*/

	/*O_GameObject* Sphere = new O_GameObject();
	Sphere->AddComponentOfClass<C_StaticMeshComponent>();
	Sphere->AddComponentOfClass<C_PhysicsComponent>();
	for (auto& mesh : Sphere->GetComponentsOfClass<C_StaticMeshComponent>())
	{
		mesh->SetMesh(sphere);
		mesh->SetTexture(texture);
	}
	for (auto comp : Sphere->GetComponentsOfClass<C_PhysicsComponent>())
	{
		comp->SetVelocity(FVector3(0.2f, 0.0f, 0.0f));
		comp->SetAngularVelocity(FVector3(0.0f, 0.0f, 2.0f * M_PI));
		//comp->AngularInertia = 0.04;
		//FPhysicsLib::AddForce(comp, FVector3(0.0f, 0.0f, 0.5f), Sphere->GetPosition() + (FVector3(0.5f, 0.0f, 0.0f)));
	}
	Sphere->SetScale(FVector3(0.2, 0.2, 0.2));
	Sphere->SetPosition(FVector3(0.0f, 0.0f, 0.5f));
	Sphere->AddTag("S");*/

	GO_BilliardBall* BallA = new GO_BilliardBall();
	BallA->SetScale(FVector3(0.2, 0.2, 0.2));
	BallA->SetPosition(FVector3(0.6f, 0.2f, 0.0f));
	BallA->Mesh->SetTexture(texture);
	BallA->Mesh->SetMesh(sphere);
	BallA->Physics->SetVelocity(FVector3(-0.8f, 0.0f, 0.0f));
	BallA->Collider->SetComponentPosition(FVector3());
	BallA->AddTag("A");
	BallA->Physics->AngularInertia = 0.05;

	GO_BilliardBall* BallB = new GO_BilliardBall();
	BallB->SetScale(FVector3(0.2, 0.2, 0.2));
	BallB->SetPosition(FVector3(-0.6f, 0.0f, 0.0f));
	BallB->Mesh->SetTexture(planet);
	BallB->Mesh->SetMesh(sphere);
	BallB->Physics->SetVelocity(FVector3(1.2f, 0.0f, 0.0f));
	BallB->Collider->SetComponentPosition(FVector3());
	BallB->AddTag("B");
	BallB->Physics->AngularInertia = 0.05;

	GO_BilliardTable* Table = new GO_BilliardTable();


	UniformCameraObject* camera = new UniformCameraObject();
	camera->View.SetToLookAtMatrix(FVector3(0.0f, 0.0f, 8.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
	camera->Projection.SetToPerspectiveMatrix(0.0f, 800.0f / 600.0f, 0.1f, 10.0f);

	RenderInitializationData* renderData = new RenderInitializationData();
	renderData->Camera = camera;
	//renderData->LoadGameObject(VikingRoom);
	//renderData->LoadGameObject(Sphere);
	renderData->LoadGameObject(BallA);
	renderData->LoadGameObject(BallB);

	VGE_SDLManager* SDLManager = new VGE_SDLManager();
	SDLManager->Begin();
	Renderer* GameRenderer = new VulkanManager(SDLManager->GetWindow());
	SDLManager->SetRenderer(GameRenderer);

	std::set<O_GameObject*> GameObjectsSet;
	GameObjectsSet.insert(BallA); GameObjectsSet.insert(BallB); GameObjectsSet.insert(Table);
	std::vector<C_CollisionComponent*> Colliders;
	for (const auto& obj : GameObjectsSet)
	{
		for (const auto& component : obj->GetComponentsOfClass<C_CollisionComponent>())
		{
			Colliders.push_back(component);
		}
	}

	Clock* GameClock = new Clock();
	int w, h;
	try 
	{
		GameRenderer->Initialize(renderData);

		float deltaTime = GameClock->GetDeltaTimeSeconds();
		while (SDLManager->GetEvent().type != SDL_QUIT)
		{
			//For testing-------------------------------------------------------------------------
			deltaTime = GameClock->GetDeltaTimeSeconds();
			//Sphere->Update(deltaTime);
			//VikingRoom->Update(deltaTime);
			C_CollisionComponent::CheckForCollisions(Colliders);
			for (auto& gameObject : GameObjectsSet)
			{
				gameObject->Update(deltaTime);
			}
			printf("%f\n", BallA->GetPosition().Y);
			SDL_GetWindowSize(SDLManager->GetWindow(), &w, &h);
			camera->View.SetToLookAtMatrix(FVector3(0.0f, -4.0, 4.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 1.0f));
			camera->Projection.SetToPerspectiveMatrix(60.0f,  (float)w / (float)h, 0.1f, 20.0f);
			//---------------------------------------------------------------------------------------
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
	//if (VikingRoom != nullptr) delete(VikingRoom);
	//if (Sphere != nullptr) delete(Sphere);
	if (texture != nullptr) delete(texture);
	if (planet != nullptr) delete(planet);
	if (BallA != nullptr) delete(BallA);
	if (BallB != nullptr) delete(BallB);
	if (vikingRoom != nullptr) delete(vikingRoom);
	if (sphere != nullptr) delete(sphere);
	if (vikingTexture != nullptr) delete(vikingTexture);
	if (camera != nullptr) delete(camera);
	if (Table) delete(Table);
	//-------

	return 0;
}