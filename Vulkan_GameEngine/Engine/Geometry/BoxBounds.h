#ifndef BOXBOUNDS_H
#define BOXBOUNDS_H

#include "../Math/FMatrix4.h"
#include "../Math/FVector3.h"
#include "Plane.h"

class BoxBounds
{
	FVector3 Position;
	FVector3 Extent;

public:
	BoxBounds();
	BoxBounds(const FVector3& extent, const FVector3& position = FVector3());
	~BoxBounds();
	Plane box[6];

	void SetPosition(FVector3 position);
	void SetExtent(FVector3 position);

	inline FVector3 GetPosition() const { return Position; }
	inline FVector3 GetExtent() const { return Extent; }
};

#endif
