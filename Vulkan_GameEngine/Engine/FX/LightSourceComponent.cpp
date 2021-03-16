#include "LightSourceComponent.h"
#include "Level.h"
#include "LightInfo.h"

C_LightSourceComponent::C_LightSourceComponent(O_GameObject* owner) : C_TransformComponent(owner), Intensity(1.0f)
{
	LightInfo = new S_LightInfo();
	LightInfo->Position = GetComponentAbsolutePosition();
	LightInfo->Ambient = 0.005f;
	LightInfo->Diffuse = 1.0f;
	LightInfo->Specular = 0.5f;
	LightInfo->Colour = {1, 1, 1};
	LightInfo->TurnedOn = true;

	Owner->GetLevel()->AddLightSource(LightInfo);
}

C_LightSourceComponent::~C_LightSourceComponent()
{
	if (Owner) if(Owner->GetLevel()) Owner->GetLevel()->RemoveLightSource(LightInfo);
	if (LightInfo) delete(LightInfo);
}

void C_LightSourceComponent::Update(float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	LightInfo->Position = GetComponentAbsolutePosition();
}

float C_LightSourceComponent::GetAmbientMultiplier() const
{
	return LightInfo->Ambient;
}

float C_LightSourceComponent::GetDifuseMultiplier() const
{
	return LightInfo->Diffuse;
}

float C_LightSourceComponent::GetSpecularMultiplier() const
{
	return LightInfo->Specular;
}

float C_LightSourceComponent::GetAmbientIntensity() const
{
	return LightInfo->Ambient * LightInfo->TurnedOn;
}

float C_LightSourceComponent::GetDifuseIntensity() const
{
	return LightInfo->Diffuse * LightInfo->TurnedOn;
}

float C_LightSourceComponent::GetSpecularIntensity() const
{
	return LightInfo->Specular * LightInfo->TurnedOn;
}

float C_LightSourceComponent::GetIntensity() const
{
	return Intensity * LightInfo->TurnedOn;
}

FVector3 C_LightSourceComponent::GetColour() const
{
	if(LightInfo) return LightInfo->Colour;
	return FVector3();
}

bool C_LightSourceComponent::IsOn() const
{
	return LightInfo->TurnedOn;
}

E_LightType C_LightSourceComponent::GetLightType() const
{
	return static_cast<E_LightType>(LightInfo->LightType);
}

void C_LightSourceComponent::SetAmbientMultiplier(float intensity)
{
	LightInfo->Ambient = intensity;
}

void C_LightSourceComponent::SetDifuseMultiplier(float intensity)
{
	LightInfo->Diffuse = intensity;
}

void C_LightSourceComponent::SetSpecularMultiplier(float intensity)
{
	LightInfo->Specular = intensity;
}

void C_LightSourceComponent::SetIntensity(float intensity)
{
	LightInfo->Ambient /= Intensity;
	LightInfo->Diffuse /= Intensity;
	LightInfo->Specular /= Intensity;

	LightInfo->Ambient *= intensity;
	LightInfo->Diffuse *= intensity;
	LightInfo->Specular *= intensity;

	Intensity = intensity;
}

void C_LightSourceComponent::SetTurnedOn(bool isOn)
{
	if (isOn) LightInfo->TurnedOn = 1.0f;
	else LightInfo->TurnedOn = -1.0f;
}

void C_LightSourceComponent::SetColour(FVector3 color)
{
	LightInfo->Colour = color;
}

void C_LightSourceComponent::SetLightType(E_LightType type)
{
	LightInfo->LightType = type;
}
