#ifndef VERTEX_H
#define VERTEX_H

#include "Math/FVector3.h"

struct S_Vertex
{
	FVector3 Position, TextureCoordinates, Normal;

	inline bool operator== (const S_Vertex& vertex) const { return vertex.Position == Position && vertex.TextureCoordinates == TextureCoordinates && vertex.Normal == Normal; }
	inline bool operator!= (const S_Vertex& vertex) const { return !(*this == vertex); }
};

#endif
