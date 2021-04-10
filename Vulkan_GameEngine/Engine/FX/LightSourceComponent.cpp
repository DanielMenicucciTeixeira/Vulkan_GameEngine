#include "LightSourceComponent.h"
#include "Level.h"
#include "Math/FVector3.h"
#include "LevelGraph.h"
#include <algorithm>

C_LightSourceComponent::C_LightSourceComponent(O_GameObject* owner) : C_TransformComponent(owner), Intensity(1.0f)
{
	LevelGraph::GetInstance()->AddLight(LightInfo, LightIndex);
	SetAmbientMultiplier(0.005f);
	SetDiffuseMultiplier(1.0f);
	SetSpecularMultiplier(0.5f);
	SetColour(FVector3(1));
	SetLightType(E_LightType::POINT_LIGHT);
	SetTurnedOn(true);

}

C_LightSourceComponent::~C_LightSourceComponent()
{
	LevelGraph::GetInstance()->RemoveLight(LightIndex);
}

void C_LightSourceComponent::Update(float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	const auto& position = GetComponentAbsolutePosition();
	(*LightInfo)[0][0] = position.X;
	(*LightInfo)[0][1] = position.Y;
	(*LightInfo)[0][2] = position.Z;

}

float C_LightSourceComponent::GetAmbientMultiplier() const
{
	return (*LightInfo)[1][0];
}

float C_LightSourceComponent::GetDiffuseMultiplier() const
{
	return (*LightInfo)[1][1];
}

float C_LightSourceComponent::GetSpecularMultiplier() const
{
	return (*LightInfo)[1][2];
}

float C_LightSourceComponent::GetAmbientIntensity() const
{
	return (*LightInfo)[1][0] * (*LightInfo)[3][1];
}

float C_LightSourceComponent::GetDiffuseIntensity() const
{
	return (*LightInfo)[1][1] * (*LightInfo)[3][1];
}

float C_LightSourceComponent::GetSpecularIntensity() const
{
	return (*LightInfo)[1][2] * (*LightInfo)[3][1];
}

float C_LightSourceComponent::GetIntensity() const
{
	return Intensity * (*LightInfo)[3][1];
}

FVector3 C_LightSourceComponent::GetColour() const
{
	if(LightInfo) return FVector3((*LightInfo)[2][0], (*LightInfo)[2][1], (*LightInfo)[2][2]);
	return FVector3();
}

bool C_LightSourceComponent::IsOn() const
{
	return (*LightInfo)[3][1];
}

E_LightType C_LightSourceComponent::GetLightType() const
{
	return static_cast<E_LightType>((*LightInfo)[3][0]);
}

void C_LightSourceComponent::SetAmbientMultiplier(float intensity)
{
	(*LightInfo)[1][0] = intensity;
}

void C_LightSourceComponent::SetDiffuseMultiplier(float intensity)
{
	(*LightInfo)[1][1] = intensity;
}

void C_LightSourceComponent::SetSpecularMultiplier(float intensity)
{
	(*LightInfo)[1][2] = intensity;
}

void C_LightSourceComponent::SetIntensity(float intensity)
{
	(*LightInfo)[1][0] = (*LightInfo)[1][0] / Intensity;
	(*LightInfo)[1][1] = (*LightInfo)[1][1] / Intensity;
	(*LightInfo)[1][2] = (*LightInfo)[1][2] / Intensity;

	(*LightInfo)[1][0] *= intensity;
	(*LightInfo)[1][1] *= intensity;
	(*LightInfo)[1][2] *= intensity;

	Intensity = intensity;
}

void C_LightSourceComponent::SetTurnedOn(bool isOn)
{
	if (isOn) (*LightInfo)[3][1] = 1.0f;
	else (*LightInfo)[3][1] = -1.0f;
}

void C_LightSourceComponent::SetColour(FVector3 color)
{
	(*LightInfo)[2][0] = color.X;
	(*LightInfo)[2][1] = color.Y;
	(*LightInfo)[2][2] = color.Z;
}

void C_LightSourceComponent::SetLightType(E_LightType type)
{
	(*LightInfo)[3][0] = static_cast<float>(type);
}
