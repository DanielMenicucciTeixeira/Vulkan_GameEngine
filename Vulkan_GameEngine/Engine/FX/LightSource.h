#ifndef GO_LIGHTSOURCE_H
#define GO_LIGHTSOURCE_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class C_LightSourceComponent;
class FVector3;
class FMatrix4;
enum E_LightType;

class GO_LightSource : public O_GameObject
{
public:
	GO_LightSource(std::string name = "");
	virtual ~GO_LightSource();

	float GetAmbientMultiplier() const;
	float GetDifuseMultiplier() const;
	float GetSpecularMultiplier() const;

	float GetAmbientIntensity() const;
	float GetDifuseIntensity() const;

	float GetIntensity() const;
	FVector3 GetColour() const;
	bool IsOn() const;
	const FMatrix4* GetLightInfo() const;
	E_LightType GetLightType() const;
	//Setters

	void SetAmbientMultiplier(float intensity);
	void SetDifuseMultiplier(float intensity);
	void SetSpecularMultiplier(float intensity);
	void SetIntensity(float intensity);
	void SetColour(FVector3 color);
	void SetTurnedOn(bool isOn);
	void SetLightType(E_LightType type);

protected:
	C_LightSourceComponent* Light;
};
#endif
