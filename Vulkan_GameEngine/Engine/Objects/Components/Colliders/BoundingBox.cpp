#include "BoundingBox.h"
#include "Renderers/RenderObject.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"
#include "Core/CollisionHandler.h"

C_BoundingBox::C_BoundingBox(O_GameObject* owner, ECollisionType type) : C_CollisionComponent(owner, type), O_Component(owner)
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
	boxBounds.SetPosition(GetComponentPosition());
}

void C_BoundingBox::Update(float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);
	if (!GetIsStatic()) {
		boxBounds.SetPosition(GetComponentAbsolutePosition());
	}
}

void C_BoundingBox::PostUpdate(float deltaTime)
{
	if (!GetIsStatic() && CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->AABBSpatialCheck(this);
	}
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
