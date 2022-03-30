#include "BoxBounds.h"

BoxBounds::BoxBounds() {
	Extent.X = 1.0f;
	Extent.Y = 1.0f;
	Extent.Z = 1.0f;

	box[0] = Plane(1, 0, 0, 0);
	box[1] = Plane(0, 1, 0, 0);
	box[2] = Plane(0, 0, 1, 0);
	box[3] = Plane(1, 0, 0, Extent.X);
	box[4] = Plane(0, 1, 0, Extent.Y);
	box[5] = Plane(0, 0, 1, Extent.Z);

	Position = FVector3();
}

BoxBounds::BoxBounds(const FVector3& extent, const FVector3& position) {
	Extent = extent;
	Position = position;

	box[0] = Plane(1, 0, 0, position.X);
	box[1] = Plane(0, 1, 0, position.Y);
	box[2] = Plane(0, 0, 1, position.Z);
	box[3] = Plane(1, 0, 0, extent.X + position.X);
	box[4] = Plane(0, 1, 0, extent.Y + position.Y);
	box[5] = Plane(0, 0, 1, extent.Z + position.Z);
}

BoxBounds::~BoxBounds() {

}

void BoxBounds::SetPosition(FVector3 position) {
	box[0] = Plane(1, 0, 0, position.X);
	box[1] = Plane(0, 1, 0, position.Y);
	box[2] = Plane(0, 0, 1, position.Z);
	box[3] = Plane(1, 0, 0, Extent.X + position.X);
	box[4] = Plane(0, 1, 0, Extent.Y + position.Y);
	box[5] = Plane(0, 0, 1, Extent.Z + position.Z);

	Position = position;
}

void BoxBounds::SetExtent(FVector3 extent) {
	box[3] = Plane(1, 0, 0, extent.X + Position.X);
	box[4] = Plane(0, 1, 0, extent.Y + Position.Y);
	box[5] = Plane(0, 0, 1, extent.Z + Position.Z);

	Extent = extent;
}