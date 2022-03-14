#include "GO_Camera.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"


GO_Camera::GO_Camera(std::string name, FTransform transform) : O_GameObject(name)
{
	Camera = AddComponentOfClass<C_CameraComponent>();
	Camera->SetComponentTransform(transform);
	SetTransform(transform);
}

void GO_Camera::Start()
{
	O_GameObject::Start();
}

void GO_Camera::PreUpdate(float deltaTime_)
{
}

void GO_Camera::Update(float deltaTime_)
{
}

void GO_Camera::PostUpdate(float deltaTime_)
{
}
