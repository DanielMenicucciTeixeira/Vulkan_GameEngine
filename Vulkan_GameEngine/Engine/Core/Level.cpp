#include "Level.h"
#include "Objects/Components/CollisionComponent.h"
#include "Game.h"

O_Level::O_Level(Game* game) : O_Object()
{
	CurrentGame = game;
}

O_Level::~O_Level()
{
	for (const auto& object : LevelObjects) if (object) delete(object);
}

void O_Level::Start()
{
	for (const auto& object : LevelObjects) object->Start();
}

void O_Level::Update(float deltaTime)
{
	if (!CurrentGame->IsPaused()) for (const auto& object : LevelObjects) object->Update(deltaTime);
	else for (const auto& object : LevelObjects) if (object->UpdateWhenPaused) object->Update(deltaTime);
	C_CollisionComponent::CheckForCollisions(Colliders);
}

void O_Level::AddCollider(C_CollisionComponent* collider)
{
	Colliders.push_back(collider);
}