#include "GO_Camera.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Math/FVector3.h"

GO_Camera::GO_Camera(O_Level* level) : O_GameObject(level)
{
	Camera = AddComponentOfClass<C_CameraComponent>();
}

void GO_Camera::Start()
{
	O_GameObject::Start();
}
