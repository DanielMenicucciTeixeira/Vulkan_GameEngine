#include "BoxCollider.h"
#include "Objects/GameObjects/GameObject.h"
#include"Geometry/Sphere.h"
#include "CollisionHandler.h"
#include "Renderers/RenderObject.h"

void C_BoxCollider::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionBox.model = GetComponentModelMatrix();
}

void C_BoxCollider::SetComponentRotation(const FQuaternion& rotation)
{
	C_TransformComponent::SetComponentRotation(rotation);
	CollisionBox.model = GetComponentModelMatrix();
}

void C_BoxCollider::SetComponentScale(const FVector3& scale)
{
	C_TransformComponent::SetComponentScale(scale);
	CollisionBox.model = GetComponentModelMatrix();
}

void C_BoxCollider::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionBox.model = GetComponentModelMatrix();
}

S_Box C_BoxCollider::GetCollisionBox()
{
	return CollisionBox;
}

//bool C_BoxCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
//{
//	if (dynamic_cast<C_BoxCollision*>(otherCollider))
//	{
//		return false;
//	}
//	if (dynamic_cast<C_SphereCollision*>(otherCollider))
//	{
//		return SphereBoxCollision(dynamic_cast<C_SphereCollision*>(otherCollider)->GetCollisionSphere(), *CollisionBox, data);
//
//	}
//	return false;
//}

C_BoxCollider::C_BoxCollider(O_GameObject* owner) : C_CollisionComponent(owner)
{
	CollisionBox = S_Box();
	CollisionBox.Max = FVector3(5.0f, 5.0f, 5.0f);
	SetColliderType(ColliderType::S_Box);
}

C_BoxCollider::~C_BoxCollider()
{
}

void C_BoxCollider::GetDimensionsFromMesh(S_Mesh* mesh)
{
	CollisionBox.Max = mesh->Vertices[0].Position;
	CollisionBox.Min = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (CollisionBox.Min.X > vertex.Position.X) CollisionBox.Min.X = vertex.Position.X;
		else if (CollisionBox.Max.X < vertex.Position.X) CollisionBox.Max.X = vertex.Position.X;

		if (CollisionBox.Min.Y > vertex.Position.Y) CollisionBox.Min.Y = vertex.Position.Y;
		else if (CollisionBox.Max.Y < vertex.Position.Y) CollisionBox.Max.Y = vertex.Position.Y;

		if (CollisionBox.Min.Z > vertex.Position.Z) CollisionBox.Min.Z = vertex.Position.Z;
		else if (CollisionBox.Max.Y < vertex.Position.Y) CollisionBox.Max.Y = vertex.Position.Y;
	}
}

void C_BoxCollider::Start()
{
	C_CollisionComponent::Start();
	CollisionBox.model = GetComponentModelMatrix();
}

void C_BoxCollider::Update(float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);
	if (!GetIsStatic()) {
		CollisionBox.model = GetComponentModelMatrix();
	}
}

FVector3 C_BoxCollider::GetMin()
{
	FVector4 temp = GetComponentModelMatrix() * FVector4(CollisionBox.Min, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

FVector3 C_BoxCollider::GetMax()
{
	FVector4 temp = GetComponentModelMatrix() * FVector4(CollisionBox.Max, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

void C_BoxCollider::PostUpdate(float deltaTime)
{
	if (!GetIsStatic() && CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->OBBSpatialCheck(this);
	}
}
