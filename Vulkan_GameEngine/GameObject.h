#ifndef O_GAMEOBJECT_H
#define O_GAMEOBJECT_H

#include "Object.h"
#include <vector>

class FMatrix4;
class FVector3;
class FQuaternion;
class O_Component;
struct FTransform;

class O_GameObject : public O_Object
{
public:
	O_GameObject();
	~O_GameObject();

	void Update(float deltaTime) override;

	///Gettters
	inline FMatrix4* GetModelMatrixPointer() const { return ModelMatrix; }
	FTransform GetTransform() const;
	FVector3 GetPosition() const;
	FQuaternion GetRotation() const;
	FVector3 GetScale() const;

	///Setters
	void SetTransform(const FTransform& transform);
	void SetPosition(const FVector3& position);
	void SetRotation(const FQuaternion& rotation);
	void SetScale(const FVector3& scale);
	
	template<class componentClass>
	void AddComponentOfClass()
	{
		Components.push_back(new componentClass(this));
	}

	template<class componentClass>
	std::vector<componentClass*> GetComponentsOfClass()
	{
		std::vector<componentClass*> returnVector;
		for (const auto& component : Components) if (dynamic_cast<componentClass*>(component)) returnVector.push_back(component);
		return returnVector;
	}

protected:
	FMatrix4* ModelMatrix;
	FTransform* Transform;
	std::vector<O_Component*> Components;
	
};
#endif
