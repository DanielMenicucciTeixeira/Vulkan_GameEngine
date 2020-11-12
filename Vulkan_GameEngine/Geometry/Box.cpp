#include "Box.h"
#include "Plane.h"

Box::Box()
{
	width = 1.0f;
	height = 1.0f;
	depth = 1.0f;

	box[0] = new Plane(1, 0, 0, 0);
	box[1] = new Plane(0, 1, 0, 0);
	box[2] = new Plane(0, 0, 1, 0);
	box[3] = new Plane(1, 0, 0, width);
	box[4] = new Plane(0, 1, 0, height);
	box[5] = new Plane(0, 0, 1, depth);
}

Box::Box(float Width, float Height, float Depth)
{
	width = Width;
	height = Height;
	depth = Depth;

	box[0] = new Plane(1, 0, 0, 0);
	box[1] = new Plane(0, 1, 0, 0);
	box[2] = new Plane(0, 0, 1, 0);
	box[3] = new Plane(1, 0, 0, width);
	box[4] = new Plane(0, 1, 0, height);
	box[5] = new Plane(0, 0, 1, depth);
}

Box::~Box()
{
	for (auto plane : box)
	{
		delete(plane);
	}
}
