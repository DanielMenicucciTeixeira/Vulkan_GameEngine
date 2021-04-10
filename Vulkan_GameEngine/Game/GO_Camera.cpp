#include "GO_Camera.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Math/FVector3.h"

GO_Camera::GO_Camera(L_Level* level, std::string name) : O_GameObject(level, name)
{
	Camera = AddComponentOfClass<C_CameraComponent>();
}

void GO_Camera::Start()
{
	O_GameObject::Start();
}
