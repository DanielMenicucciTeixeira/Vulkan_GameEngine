#include "UserInterface.h"
#include "Objects/Components/UIComponent.h"

GO_UserInterface::GO_UserInterface(std::string name, bool isStatic) : O_GameObject(name, isStatic)
{
	Canvas = AddComponentOfClass<C_UIComponent>();
	Button = Canvas->AddUIElementOfClass<UI_Button>();
	Button->SetPosition(0, 0);
	Button->SetExtent(0.5f, 0.5f);
	Button->SetRotation(90);
}

GO_UserInterface::~GO_UserInterface()
{
}

void GO_UserInterface::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
	if (Button->IsPressed())
	{
		if (Speed < MaxSpeed)
		{
			Speed += DeltaSpeed * deltaTime;
			if (Speed > MaxSpeed) Speed = MaxSpeed;
		}
	}
	else if (Speed > 0)
	{
		Speed -= DeltaSpeed * deltaTime;
		if (Speed < 0) Speed = 0;
	}

	if(Speed > 0) Button->SetRotation(Button->GetRotationAngle() + Speed * deltaTime);
}
