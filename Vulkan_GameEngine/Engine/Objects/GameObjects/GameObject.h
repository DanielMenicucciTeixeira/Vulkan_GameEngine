#ifndef O_GAMEOBJECT_H
#define O_GAMEOBJECT_H

#include "../Object.h"
#include "Core/DebugLogger.h"
#include <string>
#include <vector>

class FMatrix4;
class FVector3;
class FQuaternion;
class O_Component;
struct FTransform;
class C_TransformComponent;
class LevelGraph;

class O_GameObject : public O_Object
{
public:
	O_GameObject(std::string name = "", bool isStatic = false);
	~O_GameObject();

	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void Start() override;

	///Gettters
	FTransform GetTransform() const;
	FTransform* GetTransformReference() const;
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
	componentClass* AddComponentOfClass()//TODO add assert function
	{
		componentClass* component = new componentClass(this);
		//Debug check
		if (component == nullptr) {
			DebugLogger::Error("Not child of Component class", "GameObject.h", __LINE__);
			delete component;
			component = nullptr;
			return nullptr;
		}

		Components.insert(component);
		return component;
	}

	//Used to get one component of a class
	template<class componentClass>
	componentClass* GetComponentOfClass() {
		for (const auto& component : Components) {
			if (componentClass* ptr = dynamic_cast<componentClass*>(component))
			{
				return ptr;
			}
		}
		DebugLogger::Error("Component does not exist", "GameObject.h", __LINE__);
		return nullptr;
	}


	//Used to get all components of a class
	template<class componentClass>
	std::set<componentClass*> GetComponentsOfClass()//TODO add assert function
	{
		std::set<componentClass*> returnSet;
		for (const auto& component : Components) if (dynamic_cast<componentClass*>(component)) returnSet.insert(dynamic_cast<componentClass*>(component));
		return returnSet;
	}

	void RemoveComponent(O_Component* component);

	//Assistant functions for static implementation
	bool GetIsStatic();

protected:
	C_TransformComponent* Root = nullptr;
	std::set<O_Component*> Components;

private:
	friend LevelGraph;
	
};
#endif
