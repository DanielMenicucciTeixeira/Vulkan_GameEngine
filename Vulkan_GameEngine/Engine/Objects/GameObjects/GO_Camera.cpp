#include "GO_Camera.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"


GO_Camera::GO_Camera(std::string name, FTransform transform) : O_GameObject(name)
{
	Camera = new C_CameraComponent(this);
	SetTransform(transform);
}

void GO_Camera::Start()
{
	O_GameObject::Start();
	Camera->Start();
}

void GO_Camera::PreUpdate(float deltaTime_)
{
}

void GO_Camera::Update(float deltaTime_)
{
	Camera->Update(deltaTime_);
}

void GO_Camera::PostUpdate(float deltaTime_)
{
}

void GO_Camera::SetComponentTransform(FTransform transform_)
{
	O_GameObject::SetTransform(transform_);
	Camera->UpdateView();
}
