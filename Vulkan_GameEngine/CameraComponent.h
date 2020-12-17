#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "TransformComponent.h"

struct UniformCameraObject;

struct S_FieldOfView
{
	float Angle = 45.0f;
	float NearPlane = 0.1f;
	float FarPlane = 10.0f;
};

class C_CameraComponent : public C_TransformComponent
{
protected:
	UniformCameraObject* UCO;

public:
	S_FieldOfView FildOfView;

	void Update(float deltaTime) override;
	C_CameraComponent(O_GameObject* owner);
	~C_CameraComponent();
};
#endif

