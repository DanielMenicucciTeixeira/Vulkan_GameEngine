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

	//Theory
	// 
	// using enum to remind collider what type it is.
	// cast to type. Could use similar system to component casting?
	// function selection. This could be done with a switch?
	//
	// For collision detection with other colliders
	// could just return using the get?
	// 
	// 
	// also use case for Detection is for when you want to compare two colliders and you have them.
	// handler is used for seeing if there are any colliders that are colliding in a scene. (like an explosion)
	//


	//Attempt to make it actually check the objects in the section?
/*if (ray.IsColliding(&obj->)) {
	if (ray_.intersectionDist < shortestDistance) {
		result = obj;
		shortestDistance = ray_.intersectionDist;
	}
}
*/
	return S_CollisionData();
}

std::vector<S_CollisionData> CollisionHandler::GetSphereCollision(Sphere& sphere)
{

	for (auto coll : scenePartition->GetCollision(sphere)) {
		//TODO: Collision Detection here
	}

	return std::vector<S_CollisionData>();
}


