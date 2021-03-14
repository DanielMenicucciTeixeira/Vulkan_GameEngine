#include "GO_Camera.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Math/FVector3.h"

GO_Camera::GO_Camera(O_Level* level) : O_GameObject(level)
{
	Camera = AddComponentOfClass<C_CameraComponent>();
	Movement = AddComponentOfClass<C_MovementComponent>();
}

void GO_Camera::Start()
{
	//Movement->SetAngularVelocity(FVector3(0.0f, 0.0f, 1.0f));
	O_GameObject::Start();
}
