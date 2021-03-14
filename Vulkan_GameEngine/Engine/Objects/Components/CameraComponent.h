#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "TransformComponent.h"

struct UniformCameraObject;
class FMatrix4;

struct S_FieldOfView
{
	float Angle = 45.0f;
	float NearPlane = 2.0f;
	float FarPlane = 50.0f;
};

class C_CameraComponent : public C_TransformComponent
{
protected:
	UniformCameraObject* UCO;
	S_FieldOfView FildOfView;

public:
	virtual void Update(const float deltaTime) override;
	virtual void Start() override;
	virtual void UpdateProjection();
	C_CameraComponent(O_GameObject* owner);
	virtual ~C_CameraComponent();

	FMatrix4 GetViewMatrix();
	FMatrix4 GetProjectionMatrix();
	inline UniformCameraObject* GetUCO() { return UCO; }
};
#endif

