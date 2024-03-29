#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "OctSpatialPartition.h"
#include "Physics/CollisionData.h"
#include <vector>
#include <memory>

class C_CollisionComponent;

class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator =(const CollisionHandler&) = delete;
	CollisionHandler& operator =(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void OnCreate(float worldSize, float depth = 3);

	//Use to do collision detection for all coliders
	void Update(float deltaTime_);

	void AddCollider(C_CollisionComponent* comp);
	void RemoveCollider(C_CollisionComponent* comp);

	//Get closest collider
	S_CollisionData GetCollisionSingleRay(Ray& ray);

	//Get all colliders
	std::vector<S_CollisionData> GetCollisionMultiRay(Ray& ray);

	//Get all objects colliding with the sphere
	std::vector<S_CollisionData> GetSphereCollision(Sphere& sphere);

	//Get all colliders colliding with the bounding box
	std::vector<S_CollisionData> GetAABBCollision(BoxBounds bounds);

	//Get all colliders colliding with the box
	std::vector<S_CollisionData> GetOBBCollision(S_Box box, FVector3 boxPos);

	//TODO:Tag filters?

	//Functions to check to see if a collider is still in the same partition.
	void AABBSpatialCheck(C_BoundingBox* collider);
	void SphereSpatialCheck(C_SphereCollider* collider);
	void OBBSpatialCheck(C_BoxCollider* collider);


private:
	CollisionHandler();
	~CollisionHandler();



	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;


	static std::vector<O_GameObject*> prevCollisions;

	static OctSpatialPartition* scenePartition;

};
#endif // !COLLISIONHANDLER_H
