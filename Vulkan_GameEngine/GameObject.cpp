#include "GameObject.h"
#include "Math/FMatrix4.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Math/FTransform.h"
#include "Component.h"

O_GameObject::O_GameObject()
{
	Transform = new FTransform();
	ModelMatrix = new FMatrix4();
}

O_GameObject::~O_GameObject()
{
	if (Transform) delete(Transform);
	if (ModelMatrix) delete(ModelMatrix);
	for (const auto& component : Components) { if(component) delete(component); }
}

void O_GameObject::Update(float deltaTime)
{
	for (const auto& component : Components) component->Update(deltaTime);
	*ModelMatrix = Transform->GetModelMatrix();
}

FTransform O_GameObject::GetTransform() const
{
	return FTransform(*Transform);
}

FVector3 O_GameObject::GetPosition() const
{
	return FVector3(*Transform->Position);
}

FQuaternion O_GameObject::GetRotation() const
{
	return FQuaternion(*Transform->Rotation);
}

FVector3 O_GameObject::GetScale() const
{
	return FVector3(*Transform->Scale);
}

void O_GameObject::SetTransform(const FTransform& transform)
{
	*Transform = transform;
}

void O_GameObject::SetPosition(const FVector3& position)
{
	*Transform->Position = position;
}

void O_GameObject::SetRotation(const FQuaternion& rotation)
{
	*Transform->Rotation = rotation;
}

void O_GameObject::SetScale(const FVector3& scale)
{
	*Transform->Scale = scale;
}