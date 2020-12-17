#ifndef C_MOVEMENTCOMPONENT_H
#define C_MOVEMENTCOMPONENT_H

#include "Component.h"

class FVector3;
class FQuaternion;

class C_MovementComponent : public O_Component
{
protected:
	bool AlwaysFaceForward;

	FVector3* Acceleration;
	FVector3* Velocity;
	FVector3* AngularAcceleration;
	FVector3* AngularVelocity;

	FVector3* AccelerationBuffer;
	FVector3* VelocityBuffer;
	FVector3* AngularAccelerationBuffer;
	FVector3* AngularVelocityBuffer;
public:
	C_MovementComponent(O_GameObject* owner);
	~C_MovementComponent();

	void Update(float deltaTime) override;

	void SetAcceleration(FVector3 acceleration);
	void SetVelocity(FVector3 velocity);
	void SetAngularAcceleration(FVector3 angularAcceleration);
	void SetAngularVelocity(FVector3 angularVelocity);

	FVector3 GetAcceleration();
	FVector3 GetVelocity();
	FVector3 GetAngularAcceleration();
	FVector3 GetAngularVelocity();
};
#endif

