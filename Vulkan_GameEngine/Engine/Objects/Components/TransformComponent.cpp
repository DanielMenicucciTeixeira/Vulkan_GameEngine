#include "TransformComponent.h"

#include "Math/FTransform.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Objects/GameObjects/GameObject.h"

C_TransformComponent::C_TransformComponent(O_GameObject* owner) : O_Component(owner)
{
	Transform = new FTransform();
}

FTransform C_TransformComponent::GetComponentTransform() const
{
	return *Transform;
}

FTransform* C_TransformComponent::GetComponentTransformReference() const
{
	return Transform;
}

FVector3 C_TransformComponent::GetComponentPosition() const
{
	return *Transform->Position;
}

FQuaternion C_TransformComponent::GetComponentRotation() const
{
	return *Transform->Rotation;
}

FVector3 C_TransformComponent::GetComponentScale() const
{
	return *Transform->Scale;
}

FTransform C_TransformComponent::GetComponentAbsoluteTransform() const
{
	if (this == Owner->GetRoot()) return *Transform;
	else return Owner->GetTransform() + *Transform;
}

FVector3 C_TransformComponent::GetComponentAbsolutePosition() const
{
	if (this == Owner->GetRoot()) return GetComponentPosition();
	else return Owner->GetPosition() + GetComponentPosition();
}

FQuaternion C_TransformComponent::GetComponentAbsoluteRotation() const
{
	if (this == Owner->GetRoot()) return GetComponentRotation();
	else return Owner->GetRotation() + GetComponentRotation();
}

FVector3 C_TransformComponent::GetComponentAbsoluteScale() const
{
	if (this == Owner->GetRoot()) return GetComponentScale();
	else return Owner->GetScale() + GetComponentScale();
}

void C_TransformComponent::SetComponentTransform(const FTransform& transform)
{
	*Transform = transform;
}

void C_TransformComponent::SetComponentPosition(const FVector3& position)
{
	Transform->SetPosition(position);
}

void C_TransformComponent::SetComponentRotation(const FQuaternion& rotation)
{
	Transform->SetRotation(rotation);
}

void C_TransformComponent::SetComponentScale(const FVector3& scale)
{
	Transform->SetScale(scale);
}
