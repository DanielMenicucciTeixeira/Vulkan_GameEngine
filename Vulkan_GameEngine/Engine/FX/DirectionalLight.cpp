#include "DirectionalLight.h"
#include "LightSourceComponent.h"
#include "DirectionalLightComponent.h"
#include "LightSourceComponent.h"
#include "Level.h"

GO_DirectionalLight::GO_DirectionalLight(L_Level* level, std::string name) : GO_LightSource(level, name)
{
	SetRoot(AddComponentOfClass<C_DirectionalLightComponent>());
	RemoveComponent(Light);
	Light = dynamic_cast<C_DirectionalLightComponent*>(Root);
}
