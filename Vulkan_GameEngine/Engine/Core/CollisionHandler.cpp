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
	S_CollisionData currentData;
	float shortestDistance = FLT_MAX;
	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(ray)) {
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::RayAABBIntersection(ray, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
			break;

		case ColliderType::Sphere:
			//TODO: Add in ray sphere collision.
			//isCollideing = CollisionDetection::Ray
			break;
		}

		if (isCollideing) {
			if (shortestDistance > ray.GetIntersectDistance()) {
				currentData = CollisionDetection::GetCollisionData();
				shortestDistance = ray.GetIntersectDistance();
			}
		}
	}

	//Theory
	// 
	// For collision detection with other colliders
	// could just return using the get?
	//

	//Attempt to make it actually check the objects in the section?
/*if (ray.IsColliding(&obj->)) {
	if (ray_.intersectionDist < shortestDistance) {
		result = obj;
		shortestDistance = ray_.intersectionDist;
	}
}
*/
	return currentData;
}

std::vector<S_CollisionData> CollisionHandler::GetSphereCollision(Sphere& sphere)
{
	std::vector<S_CollisionData> currentData;
	currentData.reserve(10);
	float shortestDistance = FLT_MAX;
	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(sphere)) {
		//TODO: Collision Detection here
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::SphereAABBIntersection(sphere, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::SphereSphereIntersection(sphere, static_cast<C_SphereCollider*>(coll)->GetCollisionSphere());
		}

		if (isCollideing) {
			currentData.push_back(CollisionDetection::GetCollisionData());
		}
	}

	return std::vector<S_CollisionData>();
}


