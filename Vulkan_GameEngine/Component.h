#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component
{
protected:
	GameObject* Parent = nullptr;
	Component();
	inline Component(GameObject* parent) { Parent = parent; }

public:
	inline void SetParent(GameObject* gameObject) { Parent = gameObject; }
	virtual void Update(float deltaTime);
};

#endif // !COMPONENT_H

