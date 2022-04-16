#ifndef VERTEX_H
#define VERTEX_H

#include "Math/FVector3.h"

struct S_Vertex
{
	S_Vertex() {};
	S_Vertex(FVector3 position, FVector3 textureCoordintes, FVector3 normal) : Position(position), TextureCoordinates(textureCoordintes), Normal(normal) {}
	FVector3 Position, TextureCoordinates, Normal;

	inline bool operator== (const S_Vertex& vertex) const { return vertex.Position == Position && vertex.TextureCoordinates == TextureCoordinates && vertex.Normal == Normal; }
	inline bool operator!= (const S_Vertex& vertex) const { return !(*this == vertex); }
};

struct HASH_Vertex
{
	size_t operator()(S_Vertex const& vertex) const 
	{
		return (HASH_FVector3()(vertex.Position) ^ (HASH_FVector3()(vertex.TextureCoordinates) << 1) >> 1) ^ (HASH_FVector3()(vertex.Normal) << 1);
	}
};

#endif
