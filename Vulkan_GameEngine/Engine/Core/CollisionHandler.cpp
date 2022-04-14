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

	//TODO: Current issue.  It might be that sphere intersection is incorrect.
	int vecLoc = 0;

	//AABB Collision
	for (auto leaves : scenePartition->GetActiveLeaves()) {
		for (auto& coll1 : leaves->GetAABBColliders()) {


			//If collider is OVERLAP do check for EndOverlap here.
			if (coll1->GetCollisionType() == ECollisionType::OVERLAP && !coll1->GetDoneOverlapCheck()) {

				std::set<C_CollisionComponent*> newOverlaps;
				for (auto& coll2 : coll1->GetPrevOverlaps()) {
					switch (coll2->GetColliderType())
					{
					case ColliderType::BoundingBox:
						if (CollisionDetection::AABBIntersection(coll1->GetBoxBounds(), dynamic_cast<C_BoundingBox*>(coll2)->GetBoxBounds())) {
							newOverlaps.insert(coll2);
						}
						else {
							coll1->OnOverlapEnd(coll2);
						}
						break;

					case ColliderType::Sphere:
						if (CollisionDetection::SphereAABBIntersection(dynamic_cast<C_SphereCollider*>(coll2)->GetCollisionSphere(), coll1->GetBoxBounds())) {
							newOverlaps.insert(coll2);
						}
						else {
							coll1->OnOverlapEnd(coll2);
						}

						break;

					case ColliderType::S_Box:
						if (CollisionDetection::AABBOBBIntersection(coll1->GetBoxBounds(), dynamic_cast<C_BoxCollider*>(coll2)->GetCollisionBox(), coll2->GetComponentAbsolutePosition())) {
							newOverlaps.insert(coll2);
						}
						else {
							coll1->OnOverlapEnd(coll2);
						}

						break;
					default:
						break;
					}
				}

				coll1->SetPrevOverlaps(newOverlaps);
				coll1->SetDoneOverlapCheck(true);
			}

			C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
			//Check AABB v AABB
			for (int j = vecLoc + 1; j < leaves->GetAABBCount(); j++) {
				C_BoundingBox* boxPtr = leaves->GetAABBColliders()[j];
				C_PhysicsComponent* physicsComp2 = boxPtr->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();


				if(coll1->CheckHasCollidedBefore(boxPtr)) {}
				else if (physicsComp == nullptr && physicsComp2 == nullptr) { }
				else if (CollisionDetection::AABBIntersection(coll1->GetBoxBounds(), boxPtr->GetBoxBounds()))
				{
					if (physicsComp != nullptr) {
						physicsComp->AABBResponse(coll1, boxPtr);
					}
					else {
						physicsComp2->AABBResponse(boxPtr, coll1);
					}

					coll1->AddToPrevOverlaps(boxPtr);
					boxPtr->AddToPrevOverlaps(coll1);
				}
				boxPtr = nullptr;
				physicsComp2 = nullptr;
			}
			vecLoc++;

			//Check AABB v Sphere
			for (auto& coll2 : leaves->GetSphereColliders()) {

				if (!coll1->CheckHasCollidedBefore(coll2)) {
					if (CollisionDetection::SphereAABBIntersection(coll2->GetCollisionSphere(), coll1->GetBoxBounds()))
					{
						//If collision then do physics response
						if (coll1->GetCollisionType() == ECollisionType::COLLISION && coll2->GetCollisionType() == ECollisionType::COLLISION) {
							if (physicsComp != nullptr) {
								physicsComp->AABBSphereResponse(coll1, coll2);
							}
							else {
								coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->AABBSphereResponse(coll1, coll2);
							}
						}

						//If overlap do overlap response
						if (coll1->GetCollisionType() == ECollisionType::OVERLAP) {
							coll1->OnOverlapBegin(CollisionDetection::GetCollisionData());

						}
						if (coll2->GetCollisionType() == ECollisionType::OVERLAP) {
							coll2->OnOverlapBegin(CollisionDetection::GetCollisionData());

						}
						coll1->AddToPrevOverlaps(coll2);
						coll2->AddToPrevOverlaps(coll1);
					}
				}
			}

			//Check AABB v OBB
			for (auto& coll2 : leaves->GetOBBColliders()) {
				if (CollisionDetection::AABBOBBIntersection(coll1->GetBoxBounds(), coll2->GetCollisionBox(), coll2->GetComponentPosition()))
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
				if (CollisionDetection::SphereOBBIntersection(coll1->GetCollisionSphere(), coll2->GetCollisionBox(), coll2->GetComponentAbsolutePosition())) {
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

void CollisionHandler::RemoveCollider(C_CollisionComponent* comp)
{
	scenePartition->RemoveCollider(comp);
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

		case ColliderType::S_Box:
			isCollideing = CollisionDetection::RayOBBIntersection(ray, static_cast<C_BoxCollider*>(coll)->GetCollisionBox());
			break;
		}

		if (isCollideing && ray.obj != coll->GetOwner()) {
			if (shortestDistance > ray.GetIntersectDistance()) {
				data = CollisionDetection::GetCollisionData();
				data.OtherCollisonComponent = coll;
				shortestDistance = ray.GetIntersectDistance();
			}
		}
	}
	return data;
}

std::vector<S_CollisionData> CollisionHandler::GetCollisionMultiRay(Ray& ray)
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

		case ColliderType::S_Box:
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

		case ColliderType::S_Box:
			isCollideing = CollisionDetection::SphereOBBIntersection(sphere, static_cast<C_BoxCollider*>(coll)->GetCollisionBox(), coll->GetComponentAbsolutePosition());
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

std::vector<S_CollisionData> CollisionHandler::GetAABBCollision(BoxBounds bounds)
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

		case ColliderType::S_Box:
			isCollideing = CollisionDetection::AABBOBBIntersection(bounds, static_cast<C_BoxCollider*>(coll)->GetCollisionBox(), coll->GetComponentAbsolutePosition());
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

std::vector<S_CollisionData> CollisionHandler::GetOBBCollision(S_Box box, FVector3 boxPos)
{
	std::vector<S_CollisionData> data;

	data.reserve(20);

	bool isCollideing = false;

	for (auto coll : scenePartition->GetCollision(box, boxPos)) {
		switch (coll->GetColliderType())
		{
		case ColliderType::BoundingBox:
			isCollideing = CollisionDetection::AABBOBBIntersection(static_cast<C_BoundingBox*>(coll)->GetBoxBounds(), box, boxPos);
			break;

		case ColliderType::Sphere:
			isCollideing = CollisionDetection::SphereOBBIntersection(static_cast<C_SphereCollider*>(coll)->GetCollisionSphere(), box, boxPos);
			break;

		case ColliderType::S_Box:
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


