#ifndef UNIFORMBUFFEROBJECT_H
#define UNIFORMBUFFEROBJECT_H

#include "Math/FMatrix4.h"

struct UniformCameraObject
{
	FMatrix4 View, Projection;

	inline bool operator== (UniformCameraObject uniformCameraObject) { return View == uniformCameraObject.View && Projection == uniformCameraObject.Projection; }
};

struct UniformBufferObject
{
	FMatrix4 Model;
	FMatrix4 View;
	FMatrix4 Projection;

	inline bool operator== (UniformBufferObject uniformBufferObject) { return View == uniformBufferObject.View && Projection == uniformBufferObject.Projection; }

	UniformBufferObject(){}

	UniformBufferObject(FMatrix4 modelMatrix, FMatrix4 viewMatrix, FMatrix4 projectionMatrix)
	{
		Model = modelMatrix;
		View = viewMatrix;
		Projection = projectionMatrix;
	}
};


#endif
