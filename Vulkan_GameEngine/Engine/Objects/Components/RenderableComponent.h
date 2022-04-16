#ifndef C_RENDERABLECOMPONENT_H
#define C_RENDERABLECOMPONENT_H

#include "TransformComponent.h"

class FMatrix4;
struct FTransform;
class C_RenderableComponent : public C_TransformComponent
{
public:
	C_RenderableComponent(O_GameObject* owner) : C_TransformComponent(owner), O_Component(owner) {}
	virtual ~C_RenderableComponent() { }
	inline bool IsVisible() const { return Visible; }
	inline bool IsHiddenInGame() const { return HiddenInGame; }
	inline void SetVisibility(const bool& isVisible) { Visible = isVisible; }
	inline void SetHiddenInGame(const bool& isHidden) { HiddenInGame = isHidden; }
	inline void Update(float deltaTime) { C_TransformComponent::Update(deltaTime); }

protected:
	bool Visible = true;
	bool HiddenInGame = false;
};
#endif // !C_RENDERABLECOMPONENT_H

