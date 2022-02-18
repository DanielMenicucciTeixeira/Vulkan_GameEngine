#include "CollisionHandler.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Geometry/Ray.h"
#include "Geometry/Box.h"
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
	S_CollisionData data;
	float shortestDistance = FLT_MAX;
	bool isCollideing = false;
	for(auto coll : scenePartition->GetCollision(ray))
	switch (coll->GetColliderType())
	{
	case ColliderType::BoundingBox:
		isCollideing = CollisionDetection::RayAABBIntersection(ray, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
		break;

	case ColliderType::Sphere:
		isCollideing = CollisionDetection::RaySphereIntersection(ray, static_cast<C_SphereCollider*>(coll)->GetCollisionSphere());
		break;

	case ColliderType::Box:
		isCollideing = CollisionDetection::RayOBBIntersection(ray, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
		break;
	}

	if (isCollideing) {
		if (shortestDistance > ray.GetIntersectDistance()) {
			data = CollisionDetection::GetCollisionData();
			shortestDistance = ray.GetIntersectDistance();
		}
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetCollisionMuliRay(Ray& ray)
{
	std::vector<S_CollisionData> data;
	
	data.reserve(20);

	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(ray))
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::RayAABBIntersection(ray, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::RaySphereIntersection(ray, static_cast<C_SphereCollider*>(coll)->GetCollisionSphere());
			break;

		case ColliderType::Box:
			isCollideing = CollisionDetection::RayOBBIntersection(ray, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
			break;
		}

	if (isCollideing) {
		data.push_back(CollisionDetection::GetCollisionData());
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetSphereCollision(Sphere& sphere)
{
	std::vector<S_CollisionData> data;
	
	data.reserve(20);

	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(sphere)) {
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::SphereAABBIntersection(sphere, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::SphereIntersection(sphere, static_cast<C_SphereCollider*>(coll)->GetCollisionSphere());
			break;

		case ColliderType::Box:
			isCollideing = CollisionDetection::SphereOBBIntersection(sphere, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
			break;
		}

		if (isCollideing) {
			data.push_back(CollisionDetection::GetCollisionData());
		}
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetAABBCollision(S_BoxBounds bounds)
{
	std::vector<S_CollisionData> data;

	data.reserve(20);

	bool isCollideing = false;

	for (auto coll : scenePartition->GetCollision(bounds)) {
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::AABBIntersection(bounds, static_cast<C_BoundingBox*>(coll)->GetBoxBounds());
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::SphereAABBIntersection(static_cast<C_SphereCollider*>(coll)->GetCollisionSphere(), bounds);
			break;

		case ColliderType::Box:
			isCollideing = CollisionDetection::AABBOBBIntersection(bounds, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
			break;
		}

		if (isCollideing) {
			data.push_back(CollisionDetection::GetCollisionData());
		}
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetOBBCollision(Box box)
{
	std::vector<S_CollisionData> data;

	data.reserve(20);

	bool isCollideing = false;

	for (auto coll : scenePartition->GetCollision(box)) {
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::AABBOBBIntersection(static_cast<C_BoundingBox*>(coll)->GetBoxBounds(), box);
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::SphereOBBIntersection(static_cast<C_SphereCollider*>(coll)->GetCollisionSphere(), box);
			break;

		case ColliderType::Box:
			isCollideing = CollisionDetection::OBBIntersection(box, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
			break;
		}

		if (isCollideing) {
			data.push_back(CollisionDetection::GetCollisionData());
		}
	}
	return data;
}


