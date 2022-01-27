#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "../Objects/GameObjects/GameObject.h"
#include "OctSpatialPartition.h"
#include <vector>
#include <memory>

class CollisionHandler
{
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator =(const CollisionHandler&) = delete;
	CollisionHandler& operator =(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;


	static std::vector<O_GameObject*> prevCollisions;

	static OctSpatialPartition* scenePartition;

};
#endif // !COLLISIONHANDLER_H
