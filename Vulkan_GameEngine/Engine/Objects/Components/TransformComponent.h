#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

struct FTransform;
class FVector3;
class FQuaternion;

class C_TransformComponent : public O_Component
{
public:
	C_TransformComponent(O_GameObject* owner);
	virtual ~C_TransformComponent() { if (Transform) delete(Transform); }
	
	FTransform GetComponentTransform() const;
	FTransform* GetComponentTransformReference() const;
	FVector3 GetComponentPosition() const;
	FQuaternion GetComponentRotation() const;
	FVector3 GetComponentScale() const;

	FTransform GetComponentAbsoluteTransform() const;
	FVector3 GetComponentAbsolutePosition() const;
	FQuaternion GetComponentAbsoluteRotation() const;
	FVector3 GetComponentAbsoluteScale() const;
	

	virtual void SetComponentTransform(const FTransform& transform);
	virtual void SetComponentPosition(const FVector3& position);
	virtual void SetComponentRotation(const FQuaternion& rotation);
	virtual void SetComponentScale(const FVector3& scale);
	inline virtual void SetUseAbsolutes(bool positoin, bool rotation, bool scale) { UseAbsolutePosition = positoin; UseAbsoluteRotation = rotation; UseAbsoluteScale = scale; }


protected:
	FTransform* Transform;
	bool UseAbsoluteRotation;
	bool UseAbsolutePosition;
	bool UseAbsoluteScale;
};

#endif
