#ifndef C_LIGHTSOURCECOMPONENT_H
#define C_LIGHTSOURCECOMPONENT_H

#include "Objects/Components/TransformComponent.h"
struct S_LightInfo;
enum E_LightType;

class C_LightSourceComponent : public C_TransformComponent
{
public:
	C_LightSourceComponent(O_GameObject* owner);
	virtual ~C_LightSourceComponent();

	virtual void Update(float deltaTime) override;

	//Getters

	float GetAmbientMultiplier() const;
	float GetDifuseMultiplier() const;
	float GetSpecularMultiplier() const;

	float GetAmbientIntensity() const;
	float GetDifuseIntensity() const;
	float GetSpecularIntensity() const;

	float GetIntensity() const;
	FVector3 GetColour() const;
	bool IsOn() const;
	inline const S_LightInfo* GetLightInfo() const { return LightInfo; }
	E_LightType GetLightType() const;

	//Setters

	void SetAmbientMultiplier(float intensity);
	void SetDifuseMultiplier(float intensity);
	void SetSpecularMultiplier(float intensity);
	void SetIntensity(float intensity);
	void SetTurnedOn(bool isOn);
	void SetColour(FVector3 color);
	void SetLightType(E_LightType type);

protected:
	S_LightInfo* LightInfo;
	float Intensity;
};
#endif

