#include "DirectionalLight.h"
#include "LightSourceComponent.h"
#include "DirectionalLightComponent.h"
#include "LightSourceComponent.h"
#include "Level.h"

GO_DirectionalLight::GO_DirectionalLight(std::string name) : GO_LightSource(name)
{
	SetRoot(AddComponentOfClass<C_DirectionalLightComponent>());
	RemoveComponent(Light);
	Light = dynamic_cast<C_DirectionalLightComponent*>(Root);
}
