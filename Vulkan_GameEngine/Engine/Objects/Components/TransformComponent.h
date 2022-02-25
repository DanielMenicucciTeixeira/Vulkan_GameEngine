#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

struct FTransform;
class FVector3;
class FQuaternion;
class FMatrix4;

class C_TransformComponent : public O_Component
{
public:
	C_TransformComponent(O_GameObject* owner, bool isStatic = false);
	virtual ~C_TransformComponent() { if (Transform) delete(Transform); }
	
	FTransform GetComponentTransform() const;
	FMatrix4 GetComponentModelMatrix() const;
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

	inline bool GetIsStatic() { return IsStatic; }

protected:
	FTransform* Transform;
	bool UseAbsoluteRotation;
	bool UseAbsolutePosition;
	bool UseAbsoluteScale;

private:
	bool IsStatic;
};

#endif
