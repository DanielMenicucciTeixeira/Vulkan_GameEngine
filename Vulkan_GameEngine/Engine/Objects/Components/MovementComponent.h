#ifndef C_MOVEMENTCOMPONENT_H
#define C_MOVEMENTCOMPONENT_H

#include "Component.h"
#include "Math/FVector3.h"

class FQuaternion;


/// <summary>
/// Responsable for moveing objects
/// </summary>
class C_MovementComponent : virtual public O_Component
{
protected:
	bool AlwaysFaceForward;

	FVector3 velocity;
	FVector3 velocityBuffer;


public:
	C_MovementComponent(O_GameObject* owner);
	virtual ~C_MovementComponent();

	void Update(const float deltaTime) override;

	void Translate(FVector3 translation_);

	void Rotate(FQuaternion rotation_);
};
#endif

