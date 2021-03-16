#include "DirectionalLight.h"
#include "LightSourceComponent.h"
#include "DirectionalLightComponent.h"
#include "LightSourceComponent.h"
#include "Level.h"

GO_DirectionalLight::GO_DirectionalLight(O_Level* level) : GO_LightSource(level)
{
	SetRoot(AddComponentOfClass<C_DirectionalLightComponent>());
	RemoveComponent(Light);
	Light = dynamic_cast<C_DirectionalLightComponent*>(Root);
}
