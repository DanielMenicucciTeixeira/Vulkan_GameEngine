#ifndef VERTEX_H
#define VERTEX_H

#include "Math/FVector3.h"

struct Vertex
{
	FVector3 Position, TextureCoordinates;

	inline bool operator== (Vertex& vertex) { return vertex.Position == Position && vertex.TextureCoordinates == TextureCoordinates; }
};

#endif
