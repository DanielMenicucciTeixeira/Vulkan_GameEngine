#ifndef GO_LIGHTSOURCE_H
#define GO_LIGHTSOURCE_H

#include "Objects/GameObjects/GameObject.h"

class C_LightSourceComponent;
class FVector3;
struct S_LightInfo;
enum E_LightType;

class GO_LightSource : public O_GameObject
{
public:
	GO_LightSource(O_Level* level = nullptr);
	virtual ~GO_LightSource();

	float GetAmbientMultiplier() const;
	float GetDifuseMultiplier() const;
	float GetSpecularMultiplier() const;

	float GetAmbientIntensity() const;
	float GetDifuseIntensity() const;

	float GetIntensity() const;
	FVector3 GetColour() const;
	bool IsOn() const;
	const S_LightInfo* GetLightInfo() const;
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
