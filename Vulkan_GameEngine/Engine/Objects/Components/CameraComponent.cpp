#include "CameraComponent.h"

void C_CameraComponent::Update(const float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
}

C_CameraComponent::C_CameraComponent(O_GameObject* owner) : C_TransformComponent(owner)
{
}

C_CameraComponent::~C_CameraComponent()
{
}
