#include "Level.h"
#include "Objects/Components/CollisionComponent.h"
#include "Game.h"
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "Renderers/Vulkan/VulkanManager.h"

O_Level::O_Level() : O_Object(), CurrentGame(nullptr), RenderData(nullptr)
{
}

O_Level::~O_Level()
{
	CleanUp();
}

bool O_Level::Initialize(Game* game)
{
	if (!game) return false;
	CurrentGame = game;
	return true;
}

void O_Level::Start()
{
	for (const auto& object : LevelObjects) object->Start();
}

void O_Level::LoadMesh(S_Mesh* mesh)
{
	if (!Meshes[mesh->Name]) Meshes[mesh->Name] = mesh;
	MeshLoader::LoadMesh(mesh->Path, mesh);
}

S_Mesh* O_Level::GetMesh(std::string meshName)
{
	return Meshes[meshName];
}

void O_Level::LoadMaterial(S_Material* material)
{
	if (!Materials[material->Name]) Materials[material->Name] = material;
	TextureLoader::LoadTexture(material->TextureDifuse->Path, material->TextureDifuse);
}

S_Material* O_Level::GetMaterial(std::string materialName)
{
	return Materials[materialName];
}

void O_Level::LoadLevelObjects()
{
	for (auto& object : LevelObjects)
	{
		O_GameObject* gameObject = dynamic_cast<O_GameObject*>(object);
		if (gameObject)
		{
			for (auto& mesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
			{
				mesh->SetTexture(Materials[mesh->GetTextureName()]->TextureDifuse);
				mesh->SetMesh(Meshes[mesh->GetMeshName()]);
			}
			RenderData->LoadGameObject(dynamic_cast<O_GameObject*>(object));
		}
	}

}

void O_Level::Update(const float deltaTime)
{
	if (!CurrentGame->IsPaused()) for (const auto& object : LevelObjects) object->Update(deltaTime);
	else for (const auto& object : LevelObjects) if (object->UpdateWhenPaused) object->Update(deltaTime);
	C_CollisionComponent::CheckForCollisions(Colliders);
}

void O_Level::Render()
{
}

void O_Level::CleanUp()
{
	for (const auto& object : LevelObjects) if (object) delete(object);

	if (RenderData) delete(RenderData);
	for (auto& mesh : Meshes) delete(mesh.second);
	for (auto& material : Materials) delete(material.second);
}

void O_Level::AddCollider(C_CollisionComponent* collider)
{
	Colliders.push_back(collider);
}
