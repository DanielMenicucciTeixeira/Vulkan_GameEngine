#ifndef C_RENDERABLECOMPONENT_H
#define C_RENDERABLECOMPONENT_H

#include "TransformComponent.h"
class FMatrix4;
struct FTransform;

class C_RenderableComponent : public C_TransformComponent
{
public:
	C_RenderableComponent(O_GameObject* owner) : C_TransformComponent(owner) {};
	~C_RenderableComponent() {};
	inline const bool IsVisible() const { return Visible; }
	inline void SetVisibility(const bool& isVisible) { Visible = isVisible; }

protected:
	bool Visible = true;
};
#endif // !C_RENDERABLECOMPONENT_H

