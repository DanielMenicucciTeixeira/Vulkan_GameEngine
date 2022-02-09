#include "CollisionHandler.h"

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
	float shortestDistance = FLT_MAX;

	for (auto coll : scenePartition->GetCollision(ray)) {
		//TODO::Add in collision detection here
	}

	//Attempt to make it actually check the objects in the section?
/*if (ray.IsColliding(&obj->)) {
	if (ray_.intersectionDist < shortestDistance) {
		result = obj;
		shortestDistance = ray_.intersectionDist;
	}
}
*/

	return ;
}


