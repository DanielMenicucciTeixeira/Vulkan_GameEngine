#ifndef BOX_H
#define BOX_H

class Plane;
#include "../Math/FVector3.h"
#include "Plane.h"

class Box
{
protected:
	FVector3 Position;
	FVector3 Extent;

public:
	Box();
	Box(const FVector3& extent, const FVector3& position = FVector3());
	~Box();
	Plane box[6];
	
	void SetPosition(FVector3 position);
	void SetExtent(FVector3 position);

	inline FVector3 GetPosition() const { return Position; }
	inline FVector3 GetExtent() const { return Extent; }
};
#endif

