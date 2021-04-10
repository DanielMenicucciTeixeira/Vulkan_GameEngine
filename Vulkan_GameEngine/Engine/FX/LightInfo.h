#ifndef LIGHTINFO_H
#define LIGHTINFO_H

#include "Math/FVector3.h"
#include "Math/FMatrix4.h"

/*enum E_LightType
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
	float LightType = static_cast<float>(E_LightType::POINT_LIGHT);
	float TurnedOn = true;

	FMatrix4 GetLightInfoMatrix()
	{
		return FMatrix4
		(
			{ Position.X, Position.Y, Position.Z, 0 }, 
			{ Ambient, Diffuse, Specular, 0 }, 
			{ Colour.X, Colour.Y, Colour.Z, 0 },
			{LightType, TurnedOn, 0, 0}
		);
	}
};*/

#endif
