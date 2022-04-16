#include "DirectionalLightComponent.h"
#include "LightInfo.h"
#include "Math/FQuaternion.h"

C_DirectionalLightComponent::C_DirectionalLightComponent(O_GameObject* owner) : C_LightSourceComponent(owner), O_Component(owner)
{
	(*LightInfo)[3][0] = E_LightType::DIRECTIONAL_LIGHT;
}

void C_DirectionalLightComponent::Update(float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	const auto& position = GetComponentAbsoluteRotation().GetForwardVector();
	(*LightInfo)[0][0] = position.X;
	(*LightInfo)[0][1] = position.Y;
	(*LightInfo)[0][2] = position.Z;
	int breakPoint = 0;
}
