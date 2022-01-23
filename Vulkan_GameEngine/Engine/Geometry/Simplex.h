#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "Math/FVector3.h"
#include "Math/FVector4.h"
#include "Math/FMatrix.h"
#include <array>

struct Simplex 
{
private:
	std::array<FVector3, 4> Vertices;
	unsigned Size;

public:
	Simplex() : Vertices() , Size(0) {}

	Simplex& operator=(std::initializer_list<FVector3> list) 
	{
		for (auto v = list.begin(); v != list.end(); v++) 
		{
			Vertices[std::distance(list.begin(), v)] = *v;
		}
		Size = list.size();

		return *this;
	}

	void PushFront(FVector3 point) 
	{
		Vertices = { point, Vertices[0], Vertices[1], Vertices[2] };
		Size = std::min(Size + 1, 4u);
	}

	inline FVector3& operator[](unsigned index) { return Vertices[index]; }
	unsigned size() const { return Size; }

	auto begin() const { return Vertices.begin(); }
	auto end()   const { return Vertices.end() - (4 - Size); }
};

#endif
