#ifndef GO_CAMERA_H
#define GO_CAMERA_H

#include "../Engine/Objects/GameObjects/GameObject.h"
#include "Math/FTransform.h"
#include "Math/FQuaternion.h"
#include "Math/FVector3.h"

class C_CameraComponent;
class C_MovementComponent;

class GO_Camera : public O_GameObject
{
public:
	GO_Camera(std::string name = "", FTransform transform = FTransform(FVector3(0,0,0), FQuaternion(), FVector3(1.0f)));
	virtual void Start();
	virtual void PreUpdate(float deltaTime_);
	virtual void Update(float deltaTime_);
	virtual void PostUpdate(float deltaTime_);
	inline C_CameraComponent* GetCamera() const { return Camera; }

	virtual void SetComponentTransform(FTransform transform_);

protected:
	C_CameraComponent* Camera;
};
#endif

