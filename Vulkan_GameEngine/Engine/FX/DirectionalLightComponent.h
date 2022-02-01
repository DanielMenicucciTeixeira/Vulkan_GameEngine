#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "LightSourceComponent.h"
class C_DirectionalLightComponent : public C_LightSourceComponent
{
public:
	C_DirectionalLightComponent(O_GameObject* owner);
	virtual void Update(float deltaTime) override;
	void SetLightType(E_LightType type) = delete;
};
#endif

