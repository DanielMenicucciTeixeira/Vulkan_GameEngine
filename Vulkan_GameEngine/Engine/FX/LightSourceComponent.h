#ifndef C_LIGHTSOURCECOMPONENT_H
#define C_LIGHTSOURCECOMPONENT_H

#include "Objects/Components/TransformComponent.h"
#include "Math/FMatrix4.h"
#include <vector>

enum E_LightType
{
	FIXED_INTENSITY_POINT_LIGHT = 0,
	POINT_LIGHT = 1,
	DIRECTIONAL_LIGHT = 2
};

class C_LightSourceComponent : public C_TransformComponent
{
public:
	C_LightSourceComponent(O_GameObject* owner);
	virtual ~C_LightSourceComponent();

	virtual void Update(float deltaTime) override;

	//Getters

	float GetAmbientMultiplier() const;
	float GetDiffuseMultiplier() const;
	float GetSpecularMultiplier() const;

	float GetAmbientIntensity() const;
	float GetDiffuseIntensity() const;
	float GetSpecularIntensity() const;

	float GetIntensity() const;
	FVector3 GetColour() const;
	bool IsOn() const;
	inline const FMatrix4* GetLightInfo() const { return LightInfo; }
	E_LightType GetLightType() const;

	//Setters
	void SetAmbientMultiplier(float intensity);
	void SetDiffuseMultiplier(float intensity);
	void SetSpecularMultiplier(float intensity);
	void SetIntensity(float intensity);
	void SetTurnedOn(bool isOn);
	void SetColour(FVector3 color);
	void SetLightType(E_LightType type);

protected:
	FMatrix4* LightInfo;
	unsigned int LightIndex;
	float Intensity;
};
#endif

