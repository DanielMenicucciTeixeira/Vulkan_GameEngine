#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object.h"

class O_GameObject;

class O_Component : public O_Object
{
public:
	inline O_Component(O_GameObject* owner) { Owner = owner; }
	inline ~O_Component() {}
	inline void SetOwner(O_GameObject* gameObject) { Owner = gameObject; }
	inline O_GameObject* GetOwner() { return Owner; }

protected:
	O_GameObject* Owner = nullptr;
};

#endif // !COMPONENT_H

