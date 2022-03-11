#include "LightSource.h"
#include "LightSourceComponent.h"
#include "Math/FVector3.h"
#include "Level.h"

GO_LightSource::GO_LightSource(std::string name) : O_GameObject(name)
{
	Light = AddComponentOfClass<C_LightSourceComponent>();
	RemoveComponent(Root);
	SetRoot(Light);
}

GO_LightSource::~GO_LightSource()
{
}

float GO_LightSource::GetAmbientMultiplier() const
{
	return Light->GetAmbientMultiplier();
}

float GO_LightSource::GetDifuseMultiplier() const
{
	return Light->GetDiffuseMultiplier();
}

float GO_LightSource::GetSpecularMultiplier() const
{
	return Light->GetSpecularMultiplier();
}

float GO_LightSource::GetAmbientIntensity() const
{
	return Light->GetAmbientIntensity();
}

float GO_LightSource::GetDifuseIntensity() const
{
	return Light->GetDiffuseIntensity();
}

float GO_LightSource::GetIntensity() const
{
	return Light->GetIntensity();
}

FVector3 GO_LightSource::GetColour() const
{
	return Light->GetColour();
}

bool GO_LightSource::IsOn() const
{
	return Light->IsOn();
}

const FMatrix4* GO_LightSource::GetLightInfo() const
{
	return Light->GetLightInfo();
}

E_LightType GO_LightSource::GetLightType() const
{
	return Light->GetLightType();
}

void GO_LightSource::SetAmbientMultiplier(float intensity)
{
	Light->SetAmbientMultiplier(intensity);
}

void GO_LightSource::SetDifuseMultiplier(float intensity)
{
	Light->SetDiffuseMultiplier(intensity);
}

void GO_LightSource::SetSpecularMultiplier(float intensity)
{
	Light->SetSpecularMultiplier(intensity);
}

void GO_LightSource::SetIntensity(float intensity)
{
	Light->SetIntensity(intensity);
}

void GO_LightSource::SetColour(FVector3 color)
{
	Light->SetColour(color);
}

void GO_LightSource::SetTurnedOn(bool isOn)
{
	Light->SetTurnedOn(isOn);
}

void GO_LightSource::SetLightType(E_LightType type)
{
	Light->SetLightType(type);
}
