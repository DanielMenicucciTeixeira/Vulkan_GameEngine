#ifndef UNIFORMCAMERAOBJECT_H
#define UNIFORMCAMERAOBJECT_H

#include "Math/FMatrix4.h"

struct UnifromCameraObject
{
	FMatrix4 View, Projection;

	inline bool operator== (UnifromCameraObject uniformCameraBuffer) { return View == uniformCameraBuffer.View && Projection == uniformCameraBuffer.Projection; }
};

struct UniformBufferObject
{
	FMatrix4 Model;
	FMatrix4 View;
	FMatrix4 Projection;
};


#endif
