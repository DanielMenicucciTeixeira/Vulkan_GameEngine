#include "BoundingBox.h"
#include "Renderers/RenderObject.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"
#include "Core/CollisionHandler.h"

C_BoundingBox::C_BoundingBox(O_GameObject* owner, ECollisionType type) : C_CollisionComponent(owner, type)
{

	//TODO: Find good way of setting this value with ease.
	SetComponentScale(1.0f);
	SetColliderType(ColliderType::BoundingBox);
	boxBounds.SetPosition(GetComponentAbsolutePosition());

}

C_BoundingBox::~C_BoundingBox()
{
}

void C_BoundingBox::Start()
{
	C_CollisionComponent::Start();
	boxBounds.SetPosition(GetComponentAbsolutePosition());
	//boxBounds.SetLocalPos(GetComponentPosition());
}

void C_BoundingBox::Update(float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);
	if (!GetIsStatic()) {
		boxBounds.SetPosition(GetComponentAbsolutePosition());
		//boxBounds.SetLocalPos(GetComponentPosition());
	}
}

void C_BoundingBox::PostUpdate(float deltaTime)
{
	if (!GetIsStatic() && CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->AABBSpatialCheck(this);
	}
}

void C_BoundingBox::GetBoundsFromMesh(S_Mesh* mesh, FVector3 pos, FVector3 scale_)
{
	FVector3 newPos = mesh->Vertices[0].Position;
	FVector3 newExtent = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (newPos.X > vertex.Position.X) newPos.X = vertex.Position.X;
		else if (newExtent.X < vertex.Position.X) newExtent.X = vertex.Position.X;

		if (newPos.Y > vertex.Position.Y) newPos.Y = vertex.Position.Y;
		else if (newExtent.Y < vertex.Position.Y) newExtent.Y = vertex.Position.Y;

		if (newPos.Z > vertex.Position.Z) newPos.Z = vertex.Position.Z;
		else if (newExtent.Z < vertex.Position.Z) newExtent.Y = vertex.Position.Z;
	}

	//boxBounds.SetLocalPos(newPos + pos);
	//boxBounds.SetLocalScale((newExtent - newPos) * scale_);
}

void C_BoundingBox::SetComponentPosition(const FVector3& position)
{
	//Do the static check here as well?
	Transform->SetPosition(position);

	///Theory
	///
	/// have a pointer to keep track of where what partiton you are in.
	/// what do we do about the time in between the two?
	/// 
	/// Call function in coll handler that takes in the colliders shape and the cell pointer.
	/// 
	/// will have to make three new functions for spatial part to take in the collider types.
	/// 
	/// if the cells don't match up then remove the pointer from the list and add it to the new one, also change the pointer adress
	/// in the collider to the new one. (could this be done in the function by changing it?  would that work?)
	/// 
}

void C_BoundingBox::SetComponentScale(const FVector3& scale_)
{
	scale = scale_;
	boxBounds.SetExtent(scale);
}

void C_BoundingBox::RefreshBox()
{
	boxBounds.SetPosition(GetComponentAbsolutePosition());
}
