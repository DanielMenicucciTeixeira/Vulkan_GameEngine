#include "DirectionalLightComponent.h"
#include "LightInfo.h"
#include "Math/FQuaternion.h"

C_DirectionalLightComponent::C_DirectionalLightComponent(O_GameObject* owner) : C_LightSourceComponent(owner)
{
	LightInfo->LightType = E_LightType::DIRECTIONAL_LIGHT;
}

void C_DirectionalLightComponent::Update(float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	LightInfo->Position = GetComponentAbsoluteRotation().GetForwardVector();
}
