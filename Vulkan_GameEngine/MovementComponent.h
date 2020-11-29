#ifndef C_MOVEMENTCOMPONENT_H
#define C_MOVEMENTCOMPONENT_H

#include "Component.h"

class FVector3;

class C_MovementComponent : public O_Component
{
public:
	C_MovementComponent(O_GameObject* owner);
	~C_MovementComponent();

	void Update(float deltaTime) override;

	///Getters
	FVector3 GetAcceleration();
	FVector3 GetVelocity();

	///Setters
	void SetAcceleration(FVector3 acceleration);
	void SetVelocity(FVector3 velocity);

	bool AlwaysFaceForward;

protected:
	FVector3* Acceleration;
	FVector3* Velocity;
};
#endif

