#include "CollisionHandler.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Geometry/Ray.h"
#include "Physics/CollisionDetection.h"


std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<O_GameObject*> CollisionHandler::prevCollisions = std::vector<O_GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

CollisionHandler::CollisionHandler() {
	prevCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
}

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr) {
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnCreate(float worldSize, float depth)
{
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize, depth);
}

void CollisionHandler::Update(float deltaTime_)
{
	scenePartition->Update(deltaTime_);
}

S_CollisionData CollisionHandler::GetCollisionSingleRay(Ray& ray)
{
	return scenePartition->GetCollision(ray, false)[0];
}

std::vector<S_CollisionData> CollisionHandler::GetCollisionMuliRay(Ray& ray)
{
	return scenePartition->GetCollision(ray, true);
}

std::vector<S_CollisionData> CollisionHandler::GetSphereCollision(Sphere& sphere)
{
	return scenePartition->GetCollision(sphere);
}

std::vector<S_CollisionData> CollisionHandler::GetAABBCollision(S_BoxBounds bounds)
{
	return std::vector<S_CollisionData>();
}


