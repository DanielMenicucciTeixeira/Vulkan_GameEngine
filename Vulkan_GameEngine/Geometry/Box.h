#ifndef BOX_H
#define BOX_H

class Plane;

class Box
{
public:
	Box();
	Box(float Width, float Height, float Depth);
	~Box();

	float width, height, depth;
	Plane* box[6];
};
#endif

