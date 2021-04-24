#include "CameraComponent.h"
#include "Renderers/RenderObject.h"
#include "SDL/SDLManager.h"
#include "SDL/Window.h"
#include "Math/FQuaternion.h"

void C_CameraComponent::Update(const float deltaTime)
{
	UCO->View.SetToLookAtMatrix(GetComponentAbsolutePosition(), GetComponentAbsolutePosition() + GetComponentAbsoluteRotation().GetForwardVector(), GetComponentAbsoluteRotation().GetUpVector());
	C_TransformComponent::Update(deltaTime);
}

void C_CameraComponent::Start()
{
	UCO->View.SetToLookAtMatrix(GetComponentAbsolutePosition(), GetComponentAbsolutePosition() + GetComponentAbsoluteRotation().GetForwardVector(), GetComponentAbsoluteRotation().GetUpVector());
	UCO->Projection.SetToPerspectiveMatrix(FildOfView.Angle, SDLManager::GetInstance()->GetWindowByName()->GetHeight() / SDLManager::GetInstance()->GetWindowByName()->GetWidth(), FildOfView.NearPlane, FildOfView.FarPlane);
	C_TransformComponent::Start();
}

void C_CameraComponent::UpdateProjection()
{
	UCO->Projection.SetToPerspectiveMatrix(FildOfView.Angle, SDLManager::GetInstance()->GetWindowByName()->GetHeight() / SDLManager::GetInstance()->GetWindowByName()->GetWidth(), FildOfView.NearPlane, FildOfView.FarPlane);
}

C_CameraComponent::C_CameraComponent(O_GameObject* owner) : C_TransformComponent(owner), UCO(new UniformCameraObject)
{
	
}

C_CameraComponent::~C_CameraComponent()
{
	if (UCO) delete(UCO);
}

FMatrix4 C_CameraComponent::GetViewMatrix()
{
	return UCO->View;
}

FMatrix4 C_CameraComponent::GetProjectionMatrix()
{
	return UCO->Projection;
}
