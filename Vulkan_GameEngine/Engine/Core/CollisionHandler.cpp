#include "CollisionHandler.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Objects/GameObjects/GameObject.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Geometry/Ray.h"
#include "Geometry/Box.h"
#include "Physics/CollisionDetection.h"


std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<O_GameObject*> CollisionHandler::prevCollisions = std::vector<O_GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

void CollisionHandler::AABBSpatialCheck(C_BoundingBox* collider)
{
	scenePartition->UpdateColliderNode(collider);
}

void CollisionHandler::SphereSpatialCheck(C_SphereCollider* collider)
{
	scenePartition->UpdateColliderNode(collider);
}

void CollisionHandler::OBBSpatialCheck(C_BoxCollider* collider)
{
	scenePartition->UpdateColliderNode(collider);
}

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

//TODO: Fill out how response occurs / is this a better way then the cast system?
//TODO: Also need to integrate the on collision, and on overlap system with this.
//TODO: cant just ignore statics else some collisions will to be detected.
void CollisionHandler::Update(float deltaTime_)
{
	int vecLoc = 0;

	//AABB Collision
	for (auto leaves : scenePartition->GetActiveLeaves()) {
		for (auto& coll1 : leaves->GetAABBColliders()) {
			C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
			//Check AABB v AABB
			for (int j = vecLoc + 1; j < leaves->GetAABBCount(); j++) {
				C_BoundingBox* boxPtr = leaves->GetAABBColliders()[j];
				if (physicsComp == nullptr && boxPtr->GetOwner()->GetComponentOfClass<C_PhysicsComponent>() == nullptr) { }
				else if (CollisionDetection::AABBIntersection(coll1->GetBoxBounds(), boxPtr->GetBoxBounds()))
				{
					if (physicsComp != nullptr) {
						physicsComp->AABBResponse(coll1, boxPtr);
					}
					else {
						boxPtr->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->AABBResponse(coll1, boxPtr);
					}
				}
				boxPtr = nullptr;
			}
			vecLoc++;

			//Check AABB v Sphere
			for (auto& coll2 : leaves->GetSphereColliders()) {
				if (CollisionDetection::SphereAABBIntersection(coll2->GetCollisionSphere(), coll1->GetBoxBounds()))
				{
					if (physicsComp != nullptr) {
						physicsComp->AABBSphereResponse(coll1, coll2);
					}
					else {
						coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->AABBSphereResponse(coll1, coll2);
					}
				}
			}

			//Check AABB v OBB
			for (auto& coll2 : leaves->GetOBBColliders()) {
				if (CollisionDetection::AABBOBBIntersection(coll1->GetBoxBounds(), coll2->GetCollisionBox()))
				{

				}
			}

			physicsComp = nullptr;
		}

		vecLoc = 0;

		//Sphere Collision
		for (auto& coll1 : leaves->GetSphereColliders()) {

			C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();

			//Check Sphere v Sphere
			for (int j = vecLoc + 1; j < leaves->GetSphereCount(); j++) {
				C_SphereCollider* spherePtr = leaves->GetSphereColliders()[j];
				if (CollisionDetection::SphereIntersection(coll1->GetCollisionSphere(), spherePtr->GetCollisionSphere())) {
					if (physicsComp != nullptr) {
						physicsComp->SphereSphereResponse(coll1, spherePtr);
					}
					else {
						spherePtr->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->SphereSphereResponse(coll1, spherePtr);
					}
				}
				spherePtr = nullptr;
			}
			vecLoc++;

			//Check Sphere v OBB
			for (auto& coll2 : leaves->GetOBBColliders()) {
				if (CollisionDetection::SphereOBBIntersection(coll1->GetCollisionSphere(), coll2->GetCollisionBox())) {
					coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->SphereOBBResponse(coll1, coll2);
				}
			}

			physicsComp = nullptr;
		}

		vecLoc = 0;

		//OBB Collision
		for (auto& coll1 : leaves->GetOBBColliders()) {

			C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();

			//Check OBB v OBB
			for (int j = vecLoc + 1; j < leaves->GetOBBCount(); j++) {
				C_BoxCollider* boxPtr = leaves->GetOBBColliders()[j];
				if (CollisionDetection::OBBIntersection(coll1->GetCollisionBox(), boxPtr->GetCollisionBox())) {

				}
				boxPtr = nullptr;
			}
			vecLoc++;
			physicsComp = nullptr;
		}
	}
}

void CollisionHandler::AddCollider(C_CollisionComponent* comp)
{
	scenePartition->AddCollider(comp);
}

S_CollisionData CollisionHandler::GetCollisionSingleRay(Ray& ray)
{
	S_CollisionData data;
	float shortestDistance = FLT_MAX;
	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(ray)) {
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
				data.OtherCollisonComponent = coll;
				shortestDistance = ray.GetIntersectDistance();
			}
		}
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetCollisionMuliRay(Ray& ray)
{
	std::vector<S_CollisionData> data;
	
	data.reserve(20);

	bool isCollideing = false;
	for (auto coll : scenePartition->GetCollision(ray)) {
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
			S_CollisionData d = CollisionDetection::GetCollisionData();
			d.OtherCollisonComponent = coll;
			data.push_back(d);
		}
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
			S_CollisionData d = CollisionDetection::GetCollisionData();
			d.OtherCollisonComponent = coll;
			data.push_back(d);
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
			S_CollisionData d = CollisionDetection::GetCollisionData();
			d.OtherCollisonComponent = coll;
			data.push_back(d);
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
			S_CollisionData d = CollisionDetection::GetCollisionData();
			d.OtherCollisonComponent = coll;
			data.push_back(d);
		}
	}
	return data;
}


