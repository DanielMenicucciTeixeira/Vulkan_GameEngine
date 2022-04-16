#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "Component.h"
#include "DebugLogger.h"
#include "Math/FVector2.h"

#include <string>
#include <set>

class FVector2;
class FMatrix4;

struct S_UIRect
{
private:
	FMatrix4* Model;
	float RotationAngle;

public:

	void SetRectExtent(const FVector2& extent);
	void SetRectExtent(float x, float y);

	void SetScreenPosition(const FVector2& position);
	void SetScreenPosition(float x, float y);

	void SetRotation(float angle);

	inline const FMatrix4* GetModel() const { return Model; }
	FVector2 GetRectExtent() const;
	FVector2 GetScreenPosition() const;
	inline float GetRotationAngle() const { return RotationAngle; }

	S_UIRect();
	virtual ~S_UIRect() { if (Model) delete(Model); }
};

class UI_Element
{
public:

	enum class E_ScreenAlignment
	{
		TOP_RIGHT,
		TOP_CENTER,
		TOP_LEFT,
		CENTER_RIGHT,
		CENTER,
		CENTER_LEFT,
		BOTTOM_RIGHT,
		BOTTOM_CENTER,
		BOTTOM_LEFT
	};

	UI_Element(class C_UIComponent* ownerComponent, UI_Element* parent = nullptr, E_ScreenAlignment alignment = E_ScreenAlignment::CENTER, FVector2 position = FVector2(), FVector2 extent = FVector2(0.5f), std::string materialName = "M_UIMaterial", const bool dynamic = true);
	
	//If called by owner, deletes this component, else forces owner to destroy it.
	void Destroy(class C_UIComponent* caller = nullptr, bool destroyChildren = true);

	E_ScreenAlignment ScreenAlignment;

	inline const class C_UIComponent* GetOwnerComponent() const { return OwnerComponent; }
	inline const std::set<UI_Element*>& GetChildren() const { return Children; }
	inline UI_Element* GetParent() const { return Parent; }
	inline const std::string& GetMaterialInstanceName() const { return MaterialInstanceName; }

	const FVector2& GetExtent() const;
	const FVector2& GetPosition(bool absolute = false) const;
	float GetRotationAngle() const;
	const bool* IsVisible() const;
	void SetVisibility(bool visibility);

	void SetExtent(const FVector2& extent);
	void SetExtent(float x, float y);

	void SetPosition(const FVector2& position, bool absolute = false);
	void SetPosition(float x, float y, bool absolute = false);

	void SetRotation(float angle);

	void Attach(UI_Element* parent);
	void Dettach();

	inline const S_UIRect* GetRect() const { return &Rect; }
	inline const std::pair<const FMatrix4*, const bool*> GetModelData() const { return std::make_pair(Rect.GetModel(), &TrueVisibility); }
	const bool Dynamic;

	friend class UI_Element;

protected:
	S_UIRect Rect;
	FVector2 Extent;
	FVector2 Position;
	UI_Element* Parent;
	class C_UIComponent* OwnerComponent;
	std::set<UI_Element*> Children;
	const std::string MaterialInstanceName;

	bool RelativeVisibility; //If false, component is not visible, if true visibility depends on Parent and Owner
	void RecalculateVisibility();

	//Actual visiblity, takes into account RealativeVisibility of self and visibilty of Parent and Owner
	//Used to speed up render checks
	bool TrueVisibility;

	virtual ~UI_Element();
};

class UI_Image : virtual public UI_Element
{
public:
	UI_Image(class C_UIComponent* ownerComponent, UI_Element* parent = nullptr, UI_Element::E_ScreenAlignment alignment = UI_Element::E_ScreenAlignment::CENTER, FVector2 position = FVector2(), FVector2 extent = FVector2(0.5f), std::string materialName = "Image_Material", const bool dynamic = true);
	virtual ~UI_Image() {}

};

class UI_Button : public UI_Image
{
	using button_function = void(*)(void*);

public:
	UI_Button(class C_UIComponent* ownerComponent, UI_Element* parent = nullptr, UI_Element::E_ScreenAlignment alignment = UI_Element::E_ScreenAlignment::CENTER, FVector2 position = FVector2(), FVector2 extent = FVector2(0.5f), std::string materialName = "Image_Material", const bool dynamic = true);
	virtual ~UI_Button() {}

	void AddOnPressedFucntion(button_function* function);
	void AddOnReleasedFucntion(button_function* function);
	inline bool IsPressed() const { return Pressed; }

protected:
	static void OnPressed(void* self, SDL_Event* event);
	static void OnReleased(void* self, SDL_Event* event);

	bool IsMouseOverButton();

	std::set<button_function*> OnPressedFunctions;
	std::set<button_function*> OnReleasedFunctions;
	
	bool Pressed;

};

class C_UIComponent : virtual public O_Component
{
public:
	C_UIComponent(O_GameObject* owner);
	virtual ~C_UIComponent();

	void DestroyElement(class UI_Element* element, bool destroyChildren = true);

	bool Visible;

	template<class elementClass>
	elementClass* AddUIElementOfClass(const bool dynamic, std::string materialName, FVector2 extent, UI_Element* parent = nullptr, FVector2 position = FVector2(), UI_Element::E_ScreenAlignment alignment = UI_Element::E_ScreenAlignment::CENTER)
	{
		static_assert(std::is_base_of<UI_Element, elementClass>::value, "elemetClass must derive from UI_Element!");
		elementClass uiElement = new elementClass(this, parent, alignment, position, extent, materialName, dynamic);
		if (!dynamic_cast<UI_Element*>(uiElement))
		{
			DebugLogger::Error("Invalid UI_Element class: " + std::string(typeid(uiElement).name()), "Objects/Components/UIComponent.h", __LINE__);
			delete(uiElement);
			uiElement = nullptr;
			return nullptr;
		}
		Elements.insert(uiElement);
		return uiElement;
	}

	template<class elementClass>
	elementClass* AddUIElementOfClass(UI_Element* parent = nullptr, FVector2 position = FVector2(), UI_Element::E_ScreenAlignment alignment = UI_Element::E_ScreenAlignment::CENTER)
	{
		static_assert(std::is_base_of<UI_Element, elementClass>::value, "elemetClass must derive from UI_Element!");
		elementClass* uiElement = new elementClass(this, parent, alignment, position);
		/*if (!dynamic_cast<UI_Element*>(uiElement))
		{
			DebugLogger::Error("Invalid UI_Element class: " + std::string(typeid(uiElement).name()), "Objects/Components/UIComponent.h", __LINE__);
			delete(uiElement);
			uiElement = nullptr;
			return nullptr;
		}*/
		Elements.insert(dynamic_cast<UI_Element*>(uiElement));
		return uiElement;
	}

protected:
	std::set<UI_Element*> Elements;
};
#endif

