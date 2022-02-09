#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "../Objects/GameObjects/GameObject.h"
#include "OctSpatialPartition.h"
#include "Physics/CollisionData.h"
#include <vector>
#include <memory>

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

	S_CollisionData GetCollisionSingleRay(Ray& ray);

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;


	static std::vector<O_GameObject*> prevCollisions;

	static OctSpatialPartition* scenePartition;

};
#endif // !COLLISIONHANDLER_H
