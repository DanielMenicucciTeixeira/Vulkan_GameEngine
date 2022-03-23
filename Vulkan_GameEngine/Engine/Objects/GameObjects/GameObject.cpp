#include "GameObject.h"
#include "Math/FMatrix4.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Math/FTransform.h"
#include "Objects/Components/TransformComponent.h"
#include "Level.h"
#include "LevelGraph.h"

O_GameObject::O_GameObject(std::string name, bool isStatic) : O_Object(name)
{
	Components.clear();

	Root = new C_TransformComponent(this, isStatic);
}

O_GameObject::~O_GameObject()
{
	for (auto& component : Components) 
	{ 
		if(component) delete(component);
	}
}

void O_GameObject::Update(float deltaTime)
{
	for (const auto& component : Components) component->Update(deltaTime);
}

void O_GameObject::PostUpdate(float deltaTime)
{
	for (const auto& component : Components) component->PostUpdate(deltaTime);
}

void O_GameObject::Start()
{
	for (const auto& component : Components) component->Start();
}

FTransform O_GameObject::GetTransform() const
{
	return FTransform(Root->GetComponentTransform());
}

FTransform* O_GameObject::GetTransformReference() const
{
	return Root->GetComponentTransformReference();
}

FVector3 O_GameObject::GetPosition() const
{
	return FVector3(Root->GetComponentPosition());
}

FQuaternion O_GameObject::GetRotation() const
{
	return FQuaternion(Root->GetComponentRotation());
}

FVector3 O_GameObject::GetScale() const
{
	return FVector3(Root->GetComponentScale());
}

void O_GameObject::SetTransform(const FTransform& transform)
{

	Root->SetComponentTransform(transform);
}

void O_GameObject::SetPosition(const FVector3& position)
{
	Root->SetComponentPosition(position);
}

void O_GameObject::SetRotation(const FQuaternion& rotation)
{
	Root->SetComponentRotation(rotation);
}

void O_GameObject::SetScale(const FVector3& scale)
{
	Root->SetComponentScale(scale);
}

void O_GameObject::SetRoot(C_TransformComponent* root)
{
	Root = root;
}

void O_GameObject::RemoveComponent(O_Component* component)
{
	if (!component || !Components.count(component)) return;
	Components.erase(component);
	delete(component);
}

bool O_GameObject::GetIsStatic()
{
	 return GetRoot()->GetIsStatic(); 
}
