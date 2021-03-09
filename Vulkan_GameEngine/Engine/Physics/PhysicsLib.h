#ifndef PHYSICSLIB_H
#define PHYSICSLIB_H

#include <vector>

class C_PhysicsComponent;
class FVector3;

class FPhysicsLib
{
public:
	static void AddForce(C_PhysicsComponent* target, FVector3 force, FVector3 location);
};
#endif

