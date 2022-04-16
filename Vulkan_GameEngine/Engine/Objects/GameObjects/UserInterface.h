#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "GameObject.h"

class C_UIComponent;
class UI_Button;

class GO_UserInterface : public O_GameObject
{
public:
	GO_UserInterface(std::string name = "", bool isStatic = false);
	virtual ~GO_UserInterface();

	virtual void Update(float deltaTime) override;

	C_UIComponent* Canvas;

protected:
	UI_Button* Button;
	float DeltaSpeed = 30.0f;
	float MaxSpeed = 720.0f;
	float Speed = 0.0f;
};
#endif

