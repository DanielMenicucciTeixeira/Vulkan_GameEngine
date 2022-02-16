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



	//Get closest collider
	S_CollisionData GetCollisionSingleRay(Ray& ray);

	//Get all colliders
	std::vector<S_CollisionData> GetCollisionMuliRay(Ray& ray);

	//Get all objects colliding with the sphere
	std::vector<S_CollisionData> GetSphereCollision(Sphere& sphere);

	//Get all colliders colliding with the bounding box
	std::vector<S_CollisionData> GetAABBCollision(S_BoxBounds bounds);

	//TODO:Tag filters?

private:
	CollisionHandler();
	~CollisionHandler();



	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;


	static std::vector<O_GameObject*> prevCollisions;

	static OctSpatialPartition* scenePartition;

};
#endif // !COLLISIONHANDLER_H
