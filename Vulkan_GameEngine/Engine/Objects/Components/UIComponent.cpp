#include "UIComponent.h"
#include "LevelGraph.h"

#include "Math/FMatrix4.h"
#include "Math/FMatrix.h"
#include "Renderers/RenderObject.h"
#include "Event/EventListener.h"
#include "Event/MouseEventHandler.h"
#include "Math/IVector2.h"
#include "CoreEngine.h"

#include <SDL.h>

//------------------------S_UIRect------------------------------------------
void S_UIRect::SetRectExtent(const FVector2& extent)
{
	SetRectExtent(extent.X, extent.Y);
}

void S_UIRect::SetRectExtent(float x, float y)
{
	Model->Colum[0][0] = x;
	Model->Colum[1][1] = y;
}

void S_UIRect::SetScreenPosition(const FVector2& position)
{
	SetScreenPosition(position.X, position.Y);
}

void S_UIRect::SetScreenPosition(float x, float y)
{
	Model->Colum[3][0] = x;
	Model->Colum[3][1] = y;
}

void  S_UIRect::SetRotation(float angle)
{
	*Model = *Model * FMatrix4(eRotationAxis::Y, angle - RotationAngle);
	RotationAngle = angle;
}

FVector2 S_UIRect::GetRectExtent() const
{
	return FVector2(Model->Colum[0][0], Model->Colum[1][1]);
}

FVector2 S_UIRect::GetScreenPosition() const
{
	return FVector2(Model->Colum[3][0], Model->Colum[3][1]);
}

S_UIRect::S_UIRect()
{
	Model = new FMatrix4
	{
		FVector4(1.0f, 0.0f, 0.0f, 0.0f),
		FVector4(0.0f, 1.0f, 0.0f, 0.0f),
		FVector4(0.0f, 0.0f, 1.0f, 0.0f),
		FVector4(0.0f, 0.0f, 0.0f, 1.0f)
	};
}
//------------------------------------------------------------------------

//-------------------------C_UIComponent----------------------------------
C_UIComponent::C_UIComponent(O_GameObject* owner) : O_Component(owner)
{
}

C_UIComponent::~C_UIComponent()
{
}

void C_UIComponent::DestroyElement(UI_Element* element, bool destroyChildren)
{
	if (!element) return;
	if (destroyChildren) for (const auto& child : element->GetChildren()) DestroyElement(child);
	else
	{
		for (auto& child : element->GetChildren())
		{
			if (element->GetParent()) child->Attach(element->GetParent());
			else child->Dettach();
		}
	}
	
	Elements.erase(element);
	element->Destroy(this, destroyChildren);
}
//------------------------------------------------------------------------

//---------------------S_UIElement----------------------------------------
UI_Element::UI_Element(class C_UIComponent* ownerComponent, UI_Element* parent, E_ScreenAlignment alignment, FVector2 position, FVector2 extent, std::string materialName, const bool dynamic) :
	OwnerComponent(ownerComponent), Parent(parent), Extent(extent), Position(position), ScreenAlignment(alignment), MaterialInstanceName(materialName), Dynamic(dynamic)
{
	if (Parent)
	{
		Rect.SetRectExtent(Extent | Parent->GetExtent());
		Rect.SetScreenPosition(Position + Parent->GetPosition());
		Parent->Children.insert(this);
	}
	else
	{

		Rect.SetRectExtent(Extent);
		Rect.SetScreenPosition(Position);
	}

	LevelGraph::GetInstance()->AddUIElement(this);
}

void UI_Element::Destroy(C_UIComponent* caller, bool destroyChildren)
{
	if (OwnerComponent == caller) delete(this);
	else OwnerComponent->DestroyElement(this);
}


UI_Element::~UI_Element()
{
	LevelGraph::GetInstance()->RemoveUIElement(this);
}

const FVector2& UI_Element::GetExtent() const
{
	return Extent;
}

const FVector2& UI_Element::GetPosition(bool absolute) const
{
	return Rect.GetScreenPosition();
}

float UI_Element::GetRotationAngle() const
{
	return Rect.GetRotationAngle();
}

const bool* UI_Element::IsVisible() const
{
	return &TrueVisibility;
}

void UI_Element::SetVisibility(bool visibility)
{
	RelativeVisibility = visibility;
	RecalculateVisibility();
}

void UI_Element::RecalculateVisibility()
{
	bool newVisibilty;
	if (RelativeVisibility)
	{
		if (Parent) newVisibilty = Parent->IsVisible();
		else newVisibilty = OwnerComponent->Visible;
	}
	else newVisibilty = false;
	if (newVisibilty != TrueVisibility)
	{
		TrueVisibility = newVisibilty;
		for (auto& child : Children) child->RecalculateVisibility();
	}
}

void UI_Element::SetExtent(const FVector2& extent)
{
	Extent = extent;
	Rect.SetRectExtent(extent);
}

void UI_Element::SetExtent(float x, float y)
{
	Extent = { x, y };
	Rect.SetRectExtent(x, y);
}

void  UI_Element::SetPosition(const FVector2& position, bool absolute)
{
	if (Parent && !absolute)
	{
		Rect.SetScreenPosition(position + Parent->GetPosition());
		Position = position;
	}
	else
	{
		Rect.SetScreenPosition(position);
		if (Parent) Position = Parent->GetPosition() - position;
		else Position = position;
	}
}
void  UI_Element::SetPosition(float x, float y, bool absolute)
{
	if (Parent && !absolute)
	{
		FVector2 parentPosition = Parent->GetPosition();
		Rect.SetScreenPosition(x + parentPosition.X, y + parentPosition.Y);
		Position = { x, y };
	}
	else
	{
		Rect.SetScreenPosition(x, y);
		if (Parent) Position = Parent->GetPosition() - FVector2( x,  y);
		else Position = { x, y };
	}
}

void UI_Element::SetRotation(float angle)
{
	Rect.SetRotation(angle);
}

void UI_Element::Attach(UI_Element* parent)
{
	if (!parent || Parent == parent) return;

	if (Parent) Parent->Children.erase(this);
	if (Children.count(parent))
	{
		if (Parent) parent->Attach(Parent);
		else parent->Dettach();
	}

	Parent = parent;
	Parent->Children.insert(this);
	Position = Rect.GetScreenPosition() - Parent->GetPosition();
	FVector2 parentExtent = Parent->GetExtent();
	Extent = Rect.GetRectExtent() | FVector2(parentExtent.X, parentExtent.Y);
}

void UI_Element::Dettach()
{
	if (!Parent) return;
	Parent->Children.erase(this);
	Parent = nullptr;
	Extent = Rect.GetRectExtent();
	Position = Rect.GetScreenPosition();
}
//------------------------------------------------------------------------

UI_Image::UI_Image(C_UIComponent* ownerComponent, UI_Element* parent, UI_Element::E_ScreenAlignment alignment, FVector2 position, FVector2 extent, std::string materialName, const bool dynamic) :
	UI_Element(ownerComponent, parent, alignment, position, extent, materialName, dynamic)
{

}

UI_Button::UI_Button(C_UIComponent* ownerComponent, UI_Element* parent, UI_Element::E_ScreenAlignment alignment, FVector2 position, FVector2 extent, std::string materialName, const bool dynamic) :
	UI_Image(ownerComponent, parent, alignment, position, extent, materialName, dynamic),
	UI_Element(ownerComponent, parent, alignment, position, extent, materialName, dynamic)
{
	EventListener::AddFunctionByInput(this, OnPressed, SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT);
	EventListener::AddFunctionByInput(this, OnReleased, SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT);
}

void UI_Button::OnPressed(void* self, SDL_Event* event)
{
	UI_Button* thisButton = ((UI_Button*)self);
	if (!thisButton->IsMouseOverButton()) return;

	thisButton->Pressed = true;

	auto iterator = thisButton->OnPressedFunctions.begin();
	while (iterator != thisButton->OnPressedFunctions.end())
	{
		if (*iterator)
		{
			(**iterator)(thisButton);
			iterator++;
		}
		else thisButton->OnPressedFunctions.erase(*iterator);
	}
}

void UI_Button::OnReleased(void* self, SDL_Event* event)
{
	UI_Button* thisButton = ((UI_Button*)self);
	if (!thisButton->Pressed) return;

	thisButton->Pressed = false;

	auto iterator = thisButton->OnReleasedFunctions.begin();
	while (iterator != thisButton->OnReleasedFunctions.end())
	{
		if (*iterator)
		{
			(**iterator)(thisButton);
			iterator++;
		}
		else thisButton->OnReleasedFunctions.erase(*iterator);
	}
}

bool UI_Button::IsMouseOverButton()
{
	FVector2 mousePosition = MouseEventHandler::GetCursorPosition();
	FVector2 windowSize = CoreEngine::GetInstance()->GetWindowSize();
	mousePosition.X /= windowSize.X;
	mousePosition.Y /= windowSize.Y;

	FVector2 halfExtent = Extent;
	FVector2 screenPosition = GetPosition();
	
	if (
		mousePosition.X >= /*screenPosition.X*/ - halfExtent.X &&
		mousePosition.X <= /*screenPosition.X*/ + halfExtent.X &&
		mousePosition.Y >= /*screenPosition.X*/ - halfExtent.Y &&
		mousePosition.Y <= /*screenPosition.X*/ + halfExtent.X
		) return true;
	else return false;
}

void UI_Button::AddOnPressedFucntion(button_function* function)
{
	OnPressedFunctions.insert(function);
}

void UI_Button::AddOnReleasedFucntion(button_function* function)
{
	OnReleasedFunctions.insert(function);
}
