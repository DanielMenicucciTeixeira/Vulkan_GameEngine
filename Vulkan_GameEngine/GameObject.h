#ifndef O_GAMEOBJECT_H
#define O_GAMEOBJECT_H

#include "Object.h"
#include <vector>

class FMatrix4;
class FVector3;
class FQuaternion;
class O_Component;
struct FTransform;
class C_TransformComponent;
class O_Level;

class O_GameObject : public O_Object
{
public:
	O_GameObject(O_Level* level = nullptr);
	~O_GameObject();

	void Update(float deltaTime) override;

	///Gettters
	FTransform GetTransform() const;
	FVector3 GetPosition() const;
	FQuaternion GetRotation() const;
	FVector3 GetScale() const;
	inline C_TransformComponent* GetRoot() const { return Root; }

	///Setters
	void SetTransform(const FTransform& transform);
	void SetPosition(const FVector3& position);
	void SetRotation(const FQuaternion& rotation);
	void SetScale(const FVector3& scale);
	void SetRoot(C_TransformComponent* root);
	
	template<class componentClass>
	componentClass* AddComponentOfClass()
	{
		componentClass* component = new componentClass(this);
		Components.insert(component);
		return component;
	}

	template<class componentClass>
	std::set<componentClass*> GetComponentsOfClass()
	{
		std::set<componentClass*> returnSet;
		for (const auto& component : Components) if (dynamic_cast<componentClass*>(component)) returnSet.insert(dynamic_cast<componentClass*>(component));
		return returnSet;
	}

	void RemoveComponent(O_Component* component);

protected:
	C_TransformComponent* Root = nullptr;
	std::set<O_Component*> Components;
	
};
#endif
