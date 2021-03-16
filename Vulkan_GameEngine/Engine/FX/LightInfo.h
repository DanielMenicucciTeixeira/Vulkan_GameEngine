#ifndef LIGHTINFO_H
#define LIGHTINFO_H

#include "Math/FVector3.h"

enum E_LightType
{
	FIXED_INTENSITY_POINT_LIGHT = 0,
	POINT_LIGHT = 1,
	DIRECTIONAL_LIGHT = 2
};

struct S_LightInfo
{
	FVector3 Position = FVector3();
	float Ambient = 0.1f;
	float Diffuse = 1.0f;
	float Specular = 0.5f;
	FVector3 Colour = FVector3(1.0f);
	float TurnedOn = true;
	float LightType = static_cast<float>(E_LightType::FIXED_INTENSITY_POINT_LIGHT);

	///The next 4 operators make sure the LightInfo class can be used as an values by APIs such as Open-GL and Vulkan
	inline const float operator[] (unsigned int index) const { return *(&Position.X + index); }//For R-values
	inline float& operator[] (unsigned int index) { return*(&Position.X + index); }//For L-Values

	inline operator const float* () const { return static_cast<const float*>(&Position.X); }//For R-Values
	inline operator float* () { return static_cast<float*>(&Position.X); }//For L-Values
};

#endif
